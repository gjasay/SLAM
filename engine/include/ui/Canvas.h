#pragma once
#include <memory>
#include <vector>
#include "Element.h"
#include "Style.h"

namespace slam::ui {
  class Canvas final : private Element {
  public:
    Canvas(int width, int height);

    StyleManager styles;

    Element *AddElement(std::unique_ptr<Element> element);
    void RemoveElement(Element *element);
    void Draw(Style style = Style(), Vector2 offset = {0,0}) override;
    void SetSize(int width, int height) const;
    void Update(float dt);
  };

}; // namespace slam::ui
