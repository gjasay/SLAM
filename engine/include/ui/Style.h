#pragma once
#include <string>
#include <unordered_map>

#include "raylib.h"

namespace slam::ui {
  class Element;

  enum class FlexDirection {
    Row,
    Column
  };

  enum class JustifyContent {
    FlexStart,
    FlexEnd,
    Center,
    SpaceBetween,
  };

  enum class AlignItems {
    FlexStart,
    FlexEnd,
    Center,
    Stretch,
  };

  struct Style {
    int borderWidth = 0;
    int padding = 0;
    Color backgroundColor = { 0, 0, 0, 0 };
    Color borderColor = { 0, 0, 0, 0 };
    Color color = { 0, 0, 0, 0 };
    int fontSize = 20;
    float borderRadius = 0.0f;
    bool flex = false;
    mutable int flexGap = 0;
    FlexDirection flexDirection = FlexDirection::Row;
    JustifyContent justifyContent = JustifyContent::FlexStart;
    AlignItems alignItems = AlignItems::FlexStart;
  };

  class StyleManager {
  public:
    class RuleBuilder {
    public:
      RuleBuilder(StyleManager& mgr, const std::string& selector)
        : manager(mgr), selector(selector) {}

      RuleBuilder& borderWidth(const int width) { style.borderWidth = width; return *this; }
      RuleBuilder& padding(const int pad) { style.padding = pad; return *this; }
      RuleBuilder& backgroundColor(const Color color) { style.backgroundColor = color; return *this; }
      RuleBuilder& borderColor(const Color color) { style.borderColor = color; return *this; }
      RuleBuilder& color(const Color color) { style.color = color; return *this; }
      RuleBuilder& fontSize(const int size) { style.fontSize = size; return * this; }
      RuleBuilder& borderRadius(const float radius) { style.borderRadius = radius; return *this; }
      RuleBuilder& flex(const bool enable) { style.flex = enable; return *this; }
      RuleBuilder& flexGap(const int gap) { style.flexGap = gap; return *this; }
      RuleBuilder& flexDirection(const FlexDirection direction) { style.flexDirection = direction; return *this; }
      RuleBuilder& justifyContent(const JustifyContent justify) { style.justifyContent = justify; return *this; }
      RuleBuilder& alignItems(const AlignItems align) { style.alignItems = align; return *this; }

      // add more setters for CSS-like properties

      ~RuleBuilder() {
        manager.rules[selector] = style;
      }

    private:
      StyleManager& manager;
      std::string selector;
      Style style;
    };

    RuleBuilder AddRule(const std::string& selector) {
      return RuleBuilder(*this, selector);
    }

    Style Resolve(const Element* e) const;

  private:
    std::unordered_map<std::string, Style> rules;
    static void merge(Style& base, const Style& add);
  };

}