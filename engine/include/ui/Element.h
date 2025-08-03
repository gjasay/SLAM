#pragma once
#include <memory>
#include <vector>
#include "Style.h"
#include "Vector.h"

namespace slam::ui {
  class Canvas;

  class Element {
  public:
    explicit Element(const Vector2 position = {0, 0}, const int width = 0, const int height = 0) {
      inlineStyle->position = position;

    }

    virtual ~Element() {}

    std::string id;
    std::vector<std::string> classes;

    Canvas *canvas = nullptr;

    std::unique_ptr<Style> inlineStyle = std::make_unique<Style>();

    Element* AddChild(std::unique_ptr<Element> child) {
      child->canvas = canvas;
      children.emplace_back(std::move(child));
      return children.back().get();
    }

    virtual void Draw(Style style, Vector2 offset) {}

  protected:
    std::vector<std::unique_ptr<Element>> children;

  private:

    void _draw(Vector2 offset = {0, 0});

    friend class Canvas;
  };

  class Panel : public Element {
  public:
    Panel(const Vector2 position, const int width, const int height) : Element(position, width, height) {}

    void Draw(Style style, Vector2 offset) override;
  };

  /*
  class Slider : public Panel {
  public:
    Slider(const Vector2 position, const int width, const int height) : Panel(position, width, height) {
      this->AddChild(make_unique<Panel>(Vector2{0, 0}, width, height))->id = "sliderTrack";
    }
  };
  */

  class Button : public Element {
    public:
    Button(const Vector2 position, const int width, const int height) : Element(position, width, height) {}

    void Draw(Style style, Vector2 offset) override;
  };

  class Text : public Element {
  public:
    Text(const std::string &text, const Vector2 position) : Element(position), _text(text) {}
    void Draw(Style style, Vector2 offset) override;
    const std::string& GetText() const { return _text; }

  private:
    std::string _text;
  };
}
