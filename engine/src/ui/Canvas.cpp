#include "ui/Canvas.h"
#include <algorithm>

namespace slam::ui {
  Element* Canvas::AddElement(std::unique_ptr<Element> element) {
    element->canvas = this;
    elements.emplace_back(std::move(element));
    return elements.back().get();
  }

  void Canvas::RemoveElement(Element* element) {
    const auto it = std::remove_if(elements.begin(), elements.end(),
      [element](const std::unique_ptr<Element>& ptr) {
        return ptr.get() == element;
      });
    elements.erase(it, elements.end());
  }

  void Canvas::Draw() const {
    for (const auto& element : elements) {
      element->_draw();
    }
  }
}
