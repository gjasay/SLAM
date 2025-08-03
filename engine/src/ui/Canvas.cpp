#include "ui/Canvas.h"
#include <algorithm>
#include <iostream>

namespace slam::ui {
  Canvas::Canvas(const int width, const int height) {
    canvas = this;
    id = "canvas";
    SetSize(width, height);
    inlineStyle->visible = false;
  }

  void Canvas::SetSize(const int width, const int height) const {
    inlineStyle->width = width;
    inlineStyle->height = height;
  }

  Element* Canvas::AddElement(std::unique_ptr<Element> element) {
    element->canvas = this;
    AddChild(std::move(element));
    return children.back().get();
  }

  void Canvas::RemoveElement(Element* element) {
    const auto it = std::remove_if(children.begin(), children.end(),
      [element](const std::unique_ptr<Element>& ptr) {
        return ptr.get() == element;
      });
    children.erase(it, children.end());
  }

  void Canvas::Draw(Style style, Vector2 offset) {
    if (!inlineStyle) {
      std::cerr << "Canvas has no inline style!" << std::endl;
      return;
    }
    const Style finalStyle = styles.Resolve(this);

    if (finalStyle.visible) {
      DrawRectangle(finalStyle.position.x, finalStyle.position.y, finalStyle.width, finalStyle.height, finalStyle.backgroundColor);
    }

    _draw();
  }
}
