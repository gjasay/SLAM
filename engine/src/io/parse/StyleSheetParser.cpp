#include "../../../include/io/parse/StyleSheetParser.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <vector>

namespace slam::io {

  void StyleSheetParser::Parse(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
      throw ParseError("Failed to open file: " + filePath);
    }

    std::string line;
    std::string currentSelector;

    while (std::getline(file, line)) {
      // Trim whitespace from both ends
      line.erase(0, line.find_first_not_of(" \t\r\n"));
      line.erase(line.find_last_not_of(" \t\r\n") + 1);

      // Skip empty lines and comments
      if (line.empty() || line[0] == '/')
        continue;

      // Start of a selector block
      if (line.back() == '{') {
        currentSelector = line.substr(0, line.length() - 1);
        currentSelector.erase(currentSelector.find_last_not_of(" \t") + 1);
      }
      // End of a selector block
      else if (line[0] == '}') {
        currentSelector.clear();
      }
      // Property line inside a selector block
      else if (!currentSelector.empty()) {
        ParseProperty(currentSelector, line);
      }
    }
  }

  // Template specializations for parsing different types
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

  void StyleSheetParser::ParseProperty(const std::string &selector, const std::string &line) {
    auto [property, value] = SplitProperty(line);
    auto builder = _styleManager.AddRule(selector);

#define STYLE_FIELD(type, name, ...)                                                                                   \
  if (property == #name)                                                                                               \
    builder.name(ParseValue<type>(value));
    STYLE_PROPERTIES
#undef STYLE_FIELD
  }

  std::pair<std::string, std::string> StyleSheetParser::SplitProperty(const std::string &line) {
    auto colonPos = line.find(':');
    if (colonPos == std::string::npos) {
      throw ParseError("Invalid property line: " + line);
    }

    std::string property = line.substr(0, colonPos);
    std::string value = line.substr(colonPos + 1);

    // Trim leading/trailing whitespace from property
    property.erase(0, property.find_first_not_of(" \t"));
    property.erase(property.find_last_not_of(" \t") + 1);

    // Trim leading whitespace and trailing whitespace/semicolons from value
    value.erase(0, value.find_first_not_of(" \t"));
    auto lastChar = value.find_last_not_of(" \t\r\n;");
    if (lastChar != std::string::npos) {
      value = value.substr(0, lastChar + 1);
    }

    return {property, value};
  }


  // Parser implementations
  Color StyleSheetParser::ParseColor(const std::string &value) {
    if (value == "WHITE")
      return ::WHITE;
    if (value == "BLACK")
      return ::BLACK;
    if (value == "GRAY")
      return ::GRAY;
    if (value == "RED")
      return ::RED;

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
