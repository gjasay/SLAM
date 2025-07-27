#include "ui/Canvas.h"
#include <algorithm>

namespace slam::ui {
  void Canvas::AddElement(std::unique_ptr<Element> element) {
    elements.emplace_back(std::move(element));
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
