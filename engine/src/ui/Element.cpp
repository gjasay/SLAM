#include "../../include/ui/Element.h"
#include "../../include/ui/Canvas.h"

namespace slam::ui {
  void Element::_draw(const Vector2 offset) {
    const Style finalStyle = canvas->styles.Resolve(this);
    Draw(finalStyle, offset);
    for (const auto &child: children) {
      child->_draw(offset + position);
    }
  }

  void Panel::Draw(const Style style, const Vector2 offset) {
    const Rectangle rect = { position.x + offset.x, position.y + offset.y, static_cast<float>(width), static_cast<float>(height) };
    DrawRectangleRec(rect, style.backgroundColor);
    DrawRectangleLinesEx(rect, style.borderWidth, style.borderColor);
  }
}

