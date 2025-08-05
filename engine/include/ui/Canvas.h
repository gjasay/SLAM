#pragma once
#include <memory>
#include "Element.h"
#include "StyleManager.h"

namespace slam::ui {

  class Canvas {
  public:
    Canvas(int width, int height) : root(std::make_unique<Element>()) {;
      root->id = "canvas";
      root->canvas = this;
      SetSize(width, height);
    }

    void Draw();
    void Update(float dt);
    void SetSize(int width, int height) {
      root->defaultStyle->width = width;
      root->defaultStyle->height = height;
    }

    Element *AddElement(std::unique_ptr<Element> element);

    StyleManager styles;

  private:
    std::unique_ptr<Element> root;

    // Recursively call OnUpdate on all elements
    void UpdateElement(Element *element, float dt);

    friend class Element;
  };

} // namespace slam::ui
