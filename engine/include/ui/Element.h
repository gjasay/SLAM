#pragma once
#include <memory>
#include <vector>
#include "raylib.h"

#include "Vector.h"

namespace slam::ui {

  class Element {
  public:
    explicit Element(const Vector2 position = {0, 0}, const int width = 0, const int height = 0)
      : position(position), width(width), height(height) {}
    virtual ~Element() = default;

    int zIndex = 0;
    bool visible = true;
    Vector2 position = {0, 0};
    int width = 0;
    int height = 0;

    void AddChild(std::unique_ptr<Element> child) {
      children.emplace_back(std::move(child));
    }

    virtual void Draw(Vector2 offset) { }

  protected:
    std::vector<std::unique_ptr<Element>> children;

  private:
    void _draw(Vector2 offset = {0, 0}) {
      Draw(offset);
      for (const auto & child : children) {
        child->_draw(offset + position);
      }
    }

    friend class Canvas;
  };

  class Panel : public Element {
  public:
    Panel(const Vector2 position, const int width, const int height) : Element(position, width, height) {}

    void Draw(Vector2 offset) override;
  };
}
