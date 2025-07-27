#pragma once
#include <string>
#include <unordered_map>

#include "raylib.h"

namespace slam::ui {
  class Element;

  struct Style {
    int borderWidth = 0;
    int padding = 0;
    Color backgroundColor = { 0, 0, 0, 0 };
    Color borderColor = { 0, 0, 0, 0 };
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