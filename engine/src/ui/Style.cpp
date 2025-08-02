#include "../include/ui/Style.h"
#include "../include/ui/Element.h"
#include <iostream>

namespace slam::ui {

  void StyleManager::merge(Style &base, const Style &add) {
    const Style defaultStyle;
#define STYLE_FIELD(type, name, ...) mergeField(base.name, add.name, defaultStyle.name);
    STYLE_PROPERTIES
#undef STYLE_FIELD
  }

  void StyleManager::mergeInline(Style &base, const Style &inlineStyle) {
    const Style defaultInline;
#define STYLE_FIELD(type, name, ...) \
  if (inlineStyle.name != defaultInline.name) \
    base.name = inlineStyle.name;
    STYLE_PROPERTIES
#undef STYLE_FIELD

  }

  Style StyleManager::Resolve(const Element *e) const {
    Style out;
    out.backgroundColor = WHITE;
    out.borderColor = BLACK;

    if (!e) {
      return out;
    }

    auto tagIt = rules.find(typeid(*e).name());
    if (tagIt != rules.end())
      merge(out, tagIt->second);

    for (const auto &cls: e->classes) {
      auto clsIt = rules.find("." + cls);
      if (clsIt != rules.end())
        merge(out, clsIt->second);
    }

    if (!e->id.empty()) {
      auto idIt = rules.find("#" + e->id);
      if (idIt != rules.end())
        merge(out, idIt->second);
    }

    if (e->inlineStyle)
      mergeInline(out, *e->inlineStyle);

    return out;
  }
}
