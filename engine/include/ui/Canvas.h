#pragma once
#include <memory>
#include <vector>
#include "Element.h"
#include "Style.h"

namespace slam::ui {
  class Canvas {
  public:
    Canvas() = default;
    ~Canvas() = default;

    StyleManager styles;

    Element* AddElement(std::unique_ptr<Element> element);
    void RemoveElement(Element* element);
    void Draw() const;

  private:
    std::vector<std::unique_ptr<Element>> elements;
  };
}