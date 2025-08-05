#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Style.h"

namespace slam::ui {
  class StyleManager {
  public:
    struct RuleWithState {
      StyleState state;
      Style style;
    };

    class RuleBuilder {
    public:
      RuleBuilder(StyleManager &manager, const std::string &selector, StyleState state)
        : manager(manager), selector(selector), state(state) {}

#define STYLE_FIELD(type, name, ...) \
RuleBuilder &name(const type &value) { \
  style.name = value; \
  return *this; \
}
      STYLE_PROPERTIES
#undef STYLE_FIELD

      ~RuleBuilder() {
        auto &rules = manager.rules;
        auto &vec = rules[selector];
        for (auto &rule : vec) {
          if (rule.state == state) {
            merge(rule.style, style);
            return;
          }
        }
        vec.push_back({state, style});
      }

    private:
      StyleManager &manager;
      std::string selector;
      Style style;
      StyleState state;
    };

    RuleBuilder AddRule(const std::string &selector, StyleState state = StyleState::Default) {
      return RuleBuilder(*this, selector, state);
    }

    Style Resolve(const Element *e, StyleState state = StyleState::Default) const;

  private:
    std::unordered_map<std::string, std::vector<RuleWithState>> rules;
    template<typename T>
    static void mergeField(T &target, const T &source, const T &defaultVal) {
      if (source != defaultVal)
        target = source;
    }

    void AddOrUpdateRule(const std::string &selector, StyleState state, const Style &style);
    static void merge(Style &base, const Style &add);
    static void mergeInline(Style &base, const Style &inlineStyle);
  };
}