#include "../../include/ui/StyleManager.h"
#include <typeinfo>
#include "../../include/ui/Element.h"

namespace slam::ui {

  void StyleManager::merge(Style &base, const Style &add) {
    const Style defaultStyle;
#define STYLE_FIELD(type, name, ...) mergeField(base.name, add.name, defaultStyle.name);
    STYLE_PROPERTIES
#undef STYLE_FIELD
  }

  Style StyleManager::Resolve(const Element *e, StyleState state) const {
    Style result;
    result.backgroundColor = WHITE;
    result.borderColor = BLACK;

    if (!e)
      return result;

    auto getStateStyle = [&e](StyleState state) -> const Style * {
      switch (state) {
        case StyleState::Default:
          return e->defaultStyle.get();
        case StyleState::Hover:
          return e->hoverStyle.get();
        case StyleState::Active:
          return e->activeStyle.get();
        case StyleState::Focus:
          return e->focusStyle.get();
        case StyleState::Disabled:
          return e->disabledStyle.get();
        default:
          return nullptr;
      }
    };

    auto applyRules = [&](const std::string &selector, StyleState st) {
      auto it = rules.find(selector);
      if (it != rules.end()) {
        for (const auto &rule: it->second) {
          if (rule.state == st) {
            merge(result, rule.style);
          }
        }
      }
    };

    if (const Style *s = getStateStyle(StyleState::Default))
      merge(result, *s);

    if (!e->id.empty()) {
      applyRules("#" + e->id, StyleState::Default);
    }
    for (const auto &cls: e->classes) {
      applyRules("." + cls, StyleState::Default);
    }
    applyRules(typeid(*e).name(), StyleState::Default);

    if (state != StyleState::Default) {
      if (const Style *s = getStateStyle(state))
        merge(result, *s);
    }

    if (state != StyleState::Default) {
      if (!e->id.empty()) {
        applyRules("#" + e->id, state);
      }
      for (const auto &cls: e->classes) {
        applyRules("." + cls, state);
      }
      applyRules(typeid(*e).name(), state);
    }

    if (e->inlineStyle)
      merge(result, *e->inlineStyle);

    return result;
  }

} // namespace slam::ui
