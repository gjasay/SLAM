#include "../../../include/io/parse/StyleSheetParser.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace slam::io {
  struct ParsedSize {
    bool isPercent = false;
    float value = 0.0f;
  };

  static ParsedSize ParseSize(const std::string &value) {
    std::string v = value;
    v.erase(0, v.find_first_not_of(" \t"));
    v.erase(v.find_last_not_of(" \t") + 1);
    ParsedSize result;
    if (v.size() > 2 && v.substr(v.size() - 2) == "px") {
      v = v.substr(0, v.size() - 2);
      v.erase(v.find_last_not_of(" \t") + 1);
    }
    size_t percentPos = v.find('%');
    if (percentPos != std::string::npos) {
      std::string num = v.substr(0, percentPos);
      num.erase(num.find_last_not_of(" \t") + 1);
      try {
        result.value = std::stof(num) / 100.0f;
        result.isPercent = true;
      } catch (...) {
        throw ParseError("Invalid percent size: " + v);
      }
    } else {
      try {
        result.value = std::stof(v);
        result.isPercent = false;
      } catch (...) {
        throw ParseError("Invalid size: " + v);
      }
    }
    return result;
  }

  void StyleSheetParser::Parse(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
      throw ParseError("Failed to open file: " + filePath);
    }

    std::string line;
    std::string currentSelector;
    ui::StyleState currentState = ui::StyleState::Default;

    while (std::getline(file, line)) {
      line.erase(0, line.find_first_not_of(" \t\r\n"));
      line.erase(line.find_last_not_of(" \t\r\n") + 1);

      if (line.empty() || line[0] == '/')
        continue;

      if (line.back() == '{') {
        std::string selector = line.substr(0, line.length() - 1);
        selector.erase(selector.find_last_not_of(" \t") + 1);

        currentState = ui::StyleState::Default;
        size_t pseudoPos = selector.find(':');

        if (pseudoPos != std::string::npos) {
          std::string baseSelector = selector.substr(0, pseudoPos);
          std::string pseudoSelector = selector.substr(pseudoPos + 1);

          baseSelector.erase(baseSelector.find_last_not_of(" \t") + 1);
          pseudoSelector.erase(0, pseudoSelector.find_first_not_of(" \t"));

          if (pseudoSelector == "hover")
            currentState = ui::StyleState::Hover;
          else if (pseudoSelector == "active")
            currentState = ui::StyleState::Active;
          else if (pseudoSelector == "focus")
            currentState = ui::StyleState::Focus;
          else if (pseudoSelector == "disabled")
            currentState = ui::StyleState::Disabled;
          else
            throw ParseError("Unknown pseudo-selector: " + pseudoSelector);

          currentSelector = baseSelector;
        } else {
          currentSelector = selector;
        }
      }

      else if (line[0] == '}') {
        currentSelector.clear();
        currentState = ui::StyleState::Default;
      }

      else if (!currentSelector.empty()) {
        ParseProperty(currentSelector, line, currentState);
      }
    }
  }

  template<>
  int StyleSheetParser::ParseValue<int>(const std::string &value) {
    return ParseInt(value);
  }
  template<>
  float StyleSheetParser::ParseValue<float>(const std::string &value) {
    return ParseFloat(value);
  }
  template<>
  bool StyleSheetParser::ParseValue<bool>(const std::string &value) {
    return ParseBool(value);
  }
  template<>
  Vector2 StyleSheetParser::ParseValue<Vector2>(const std::string &value) {
    return ParseVector2(value);
  }
  template<>
  Color StyleSheetParser::ParseValue<Color>(const std::string &value) {
    return ParseColor(value);
  }
  template<>
  ui::FlexDirection StyleSheetParser::ParseValue<ui::FlexDirection>(const std::string &value) {
    return ParseFlexDirection(value);
  }
  template<>
  ui::JustifyContent StyleSheetParser::ParseValue<ui::JustifyContent>(const std::string &value) {
    return ParseJustifyContent(value);
  }
  template<>
  ui::AlignItems StyleSheetParser::ParseValue<ui::AlignItems>(const std::string &value) {
    return ParseAlignItems(value);
  }

  void StyleSheetParser::ParseProperty(const std::string &selector, const std::string &line, ui::StyleState state) {
    auto [property, value] = SplitProperty(line);
    auto builder = _styleManager.AddRule(selector, state);

    if (property == "width") {
      auto sz = ParseSize(value);
      builder.widthIsPercent(sz.isPercent);
      if (sz.isPercent)
        builder.widthPercent(sz.value);
      else
        builder.width(sz.value);
    } else if (property == "height") {
      auto sz = ParseSize(value);
      builder.heightIsPercent(sz.isPercent);
      if (sz.isPercent)
        builder.heightPercent(sz.value);
      else
        builder.height(sz.value);
    } else {
#define STYLE_FIELD(type, name, ...)                                                                                   \
  if (property == #name)                                                                                               \
    builder.name(ParseValue<type>(value));
      STYLE_PROPERTIES
#undef STYLE_FIELD
    }
  }

  std::pair<std::string, std::string> StyleSheetParser::SplitProperty(const std::string &line) {
    auto colonPos = line.find(':');
    if (colonPos == std::string::npos) {
      throw ParseError("Invalid property line: " + line);
    }

    std::string property = line.substr(0, colonPos);
    std::string value = line.substr(colonPos + 1);

    property.erase(0, property.find_first_not_of(" \t"));
    property.erase(property.find_last_not_of(" \t") + 1);

    value.erase(0, value.find_first_not_of(" \t"));
    auto lastChar = value.find_last_not_of(" \t\r\n;");
    if (lastChar != std::string::npos) {
      value = value.substr(0, lastChar + 1);
    }

    return {property, value};
  }


  // Parser implementations
  Color StyleSheetParser::ParseColor(const std::string &value) {
    static const std::unordered_map<std::string, Color> colorMap = {
        {"WHITE", ::WHITE},       {"BLACK", ::BLACK},     {"GRAY", ::GRAY},
        {"RED", ::RED},           {"YELLOW", ::YELLOW},   {"GREEN", ::GREEN},
        {"BLUE", ::BLUE},         {"MAGENTA", ::MAGENTA}, {"LIGHTGRAY", ::LIGHTGRAY},
        {"DARKGRAY", ::DARKGRAY}, {"ORANGE", ::ORANGE},   {"PURPLE", ::PURPLE},
        {"PINK", ::PINK},         {"BROWN", ::BROWN},     {"TRANSPARENT", {0, 0, 0, 0}}};
    auto it = colorMap.find(value);
    if (it != colorMap.end())
      return it->second;

    std::vector<int> components;
    std::stringstream ss(value);
    std::string component;

    while (std::getline(ss, component, ',')) {
      component.erase(0, component.find_first_not_of(" \t"));
      component.erase(component.find_last_not_of(" \t") + 1);

      if (!component.empty()) {
        try {
          components.push_back(std::stoi(component));
        } catch (const std::exception &e) {
          throw ParseError("Invalid color component: " + component);
        }
      }
    }

    if (components.size() == 4) {
      return Color{static_cast<unsigned char>(components[0]), static_cast<unsigned char>(components[1]),
                   static_cast<unsigned char>(components[2]), static_cast<unsigned char>(components[3])};
    } else if (components.size() == 3) {
      return Color{static_cast<unsigned char>(components[0]), static_cast<unsigned char>(components[1]),
                   static_cast<unsigned char>(components[2]), 255};
    }

    throw ParseError("Invalid color format: " + value);
  }

  float StyleSheetParser::ParseFloat(const std::string &value) { return std::stof(value); }

  int StyleSheetParser::ParseInt(const std::string &value) { return std::stoi(value); }

  bool StyleSheetParser::ParseBool(const std::string &value) { return value == "true"; }

  Vector2 StyleSheetParser::ParseVector2(const std::string &value) {
    size_t comma = value.find(',');
    if (comma == std::string::npos) {
      throw ParseError("Invalid Vector2 format: " + value);
    }
    return {std::stof(value.substr(0, comma)), std::stof(value.substr(comma + 1))};
  }

  ui::FlexDirection StyleSheetParser::ParseFlexDirection(const std::string &value) {
    if (value == "row")
      return ui::FlexDirection::Row;
    if (value == "column")
      return ui::FlexDirection::Column;
    throw ParseError("Invalid FlexDirection: " + value);
  }

  ui::JustifyContent StyleSheetParser::ParseJustifyContent(const std::string &value) {
    if (value == "flexStart")
      return ui::JustifyContent::FlexStart;
    if (value == "flexEnd")
      return ui::JustifyContent::FlexEnd;
    if (value == "center")
      return ui::JustifyContent::Center;
    if (value == "spaceBetween")
      return ui::JustifyContent::SpaceBetween;
    if (value == "spaceAround")
      return ui::JustifyContent::SpaceAround;
    if (value == "spaceEvenly")
      return ui::JustifyContent::SpaceEvenly;
    throw ParseError("Invalid JustifyContent: " + value);
  }

  ui::AlignItems StyleSheetParser::ParseAlignItems(const std::string &value) {
    if (value == "flexStart")
      return ui::AlignItems::FlexStart;
    if (value == "flexEnd")
      return ui::AlignItems::FlexEnd;
    if (value == "center")
      return ui::AlignItems::Center;
    if (value == "stretch")
      return ui::AlignItems::Stretch;
    if (value == "baseline")
      return ui::AlignItems::Baseline;
    throw ParseError("Invalid AlignItems: " + value);
  }

  void StyleSheetParser::Write(const std::string &filePath) {
    throw ParseError("Writing stylesheets is not implemented yet.");
  }
} // namespace slam::io
