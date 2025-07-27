#include "../../include/ui/Element.h"
#include "../../include/ui/Canvas.h"

namespace slam::ui {
  void Element::_draw(const Vector2 offset) {
    const Style finalStyle = canvas->styles.Resolve(this);

    Vector2 childOffset = { static_cast<float>(finalStyle.padding), static_cast<float>(finalStyle.padding) };
    if (finalStyle.flex && !children.empty()) {
      for (auto &child: children) {
        child->position = childOffset;
        if (finalStyle.flexDirection == FlexDirection::Row)
          childOffset.x += child->width + finalStyle.flexGap;
        else
          childOffset.y += child->height + finalStyle.flexGap;
      }
    }

    Draw(finalStyle, offset);
    for (const auto &child: children) {
      child->_draw(offset + position);
    }
  }

  void Panel::Draw(const Style style, const Vector2 offset) {
    const Rectangle rect = { position.x + offset.x, position.y + offset.y, static_cast<float>(width), static_cast<float>(height) };

    if (style.borderRadius > 0.0f) {
      DrawRectangleRounded(rect, style.borderRadius, 0, style.backgroundColor);
      if (style.borderWidth > 0) {
        DrawRectangleRoundedLines(rect, style.borderRadius, style.borderWidth, style.borderColor);
      }
    } else {
      DrawRectangleRec(rect, style.backgroundColor);
      if (style.borderWidth > 0) {
        DrawRectangleLinesEx(rect, style.borderWidth, style.borderColor);
      }
    }
  }

  void Text::Draw(const Style style, const Vector2 offset) {
    DrawText(_text.c_str(), position.x + offset.x, position.y + offset.y, style.fontSize, style.color);
  }


}

