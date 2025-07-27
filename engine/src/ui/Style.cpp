#include "../include/ui/Style.h"
#include "../include/ui/Element.h"

namespace slam::ui {
  Style StyleManager::Resolve(const Element *e) const {
    Style out;
    out.backgroundColor = WHITE;
    out.borderColor = BLACK;

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

    return out;
  }

  void StyleManager::merge(Style &base, const Style &add) {
    // override non-default fields; extend as needed
    if (add.borderWidth != 0)
      base.borderWidth = add.borderWidth;
    if (add.padding != 0)
      base.padding = add.padding;
    if (add.backgroundColor.a != 0)
      base.backgroundColor = add.backgroundColor;
    if (add.borderColor.a != 0)
      base.borderColor = add.borderColor;
    if (add.color.a != 0)
      base.color = add.color;
    if (add.fontSize != 20)
      base.fontSize = add.fontSize;
    if (add.borderRadius != 0.0f)
      base.borderRadius = add.borderRadius;
    if (add.flex)
      base.flex = add.flex;
    if (add.flexGap != 0)
      base.flexGap = add.flexGap;
    if (add.flexDirection != FlexDirection::Row)
      base.flexDirection = add.flexDirection;
  }

}
