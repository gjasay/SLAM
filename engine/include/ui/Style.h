#pragma once
#include <string>
#include <unordered_map>
#include "StyleFields.h"
#include "Vector.h"

namespace slam::ui {
  class Element;

  enum class FlexDirection { Row, Column };
  enum class JustifyContent {
    FlexStart,
    FlexEnd,
    Center,
    SpaceBetween,
    SpaceAround,
    SpaceEvenly,
  };
  enum class AlignItems {
    FlexStart,
    FlexEnd,
    Center,
    Stretch,
    Baseline,
  };

  struct Style {
#define STYLE_FIELD(type, name, ...) type name = __VA_ARGS__;
    STYLE_PROPERTIES
#undef STYLE_FIELD
  };

  class StyleManager {
  public:
    class RuleBuilder {
    public:
      RuleBuilder(StyleManager &manager, const std::string &selector) : manager(manager), selector(selector) {}

#define STYLE_FIELD(type, name, ...)                                                                                   \
  RuleBuilder &name(const type &value) {                                                                               \
    style.name = value;                                                                                                \
    return *this;                                                                                                      \
  }
      STYLE_PROPERTIES
#undef STYLE_FIELD

      ~RuleBuilder() {
        auto &rules = manager.rules;

        if (const auto it = rules.find(selector); it != rules.end()) {
          Style merged = it->second;
          StyleManager::merge(merged, style);
          rules[selector] = merged;
        } else {
          rules[selector] = style;
        }
      }

    private:
      StyleManager &manager;
      std::string selector;
      Style style;
    };

    RuleBuilder AddRule(const std::string &selector) { return RuleBuilder(*this, selector); }
    Style Resolve(const Element *e) const;

  private:
    std::unordered_map<std::string, Style> rules;
    template<typename T>
    static void mergeField(T &target, const T &source, const T &defaultVal) {
      if (source != defaultVal)
        target = source;
    }

    static void merge(Style &base, const Style &add);
    static void mergeInline(Style &base, const Style &inlineStyle);
  };
} // namespace slam::ui
