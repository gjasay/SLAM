#pragma once
#include <memory>
#include <vector>

#include "Style.h"
#include "raylib.h"

#include "Vector.h"


namespace slam::ui {
  class Canvas;

  class Element {
  public:
    explicit Element(const Vector2 position = {0, 0}, const int width = 0, const int height = 0) :
        position(position), width(width), height(height) {}

    virtual ~Element() = default;

    std::string id;
    std::vector<std::string> classes;

    Canvas *canvas;

    int zIndex = 0;
    bool visible = true;
    Vector2 position = {0, 0};
    int width = 0;
    int height = 0;

    Element* AddChild(std::unique_ptr<Element> child) {
      child->canvas = canvas;
      children.emplace_back(std::move(child));
      return children.back().get();
    }

    virtual void Draw(Style style, Vector2 offset) {}

  protected:
    std::vector<std::unique_ptr<Element>> children;

  private:

    void _draw(const Vector2 offset = {0, 0});

    friend class Canvas;
  };

  class Panel : public Element {
  public:
    Panel(const Vector2 position, const int width, const int height) : Element(position, width, height) {}

    void Draw(Style style, Vector2 offset) override;
  };

  class Button : public Element {
    public:
    Button(const Vector2 position, const int width, const int height) : Element(position, width, height) {}

    void Draw(Style style, Vector2 offset) override;
  };

  class Text : public Element {
  public:
    Text(const std::string &text, const Vector2 position) : Element(position), _text(text) {
      height = 20;
    }
    void Draw(Style style, Vector2 offset) override;

  private:
    std::string _text;
  };
}
