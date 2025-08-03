#pragma once
#include <utility>

#include "Element.h"

namespace slam::ui {
  class Panel : public Element {
  public:
    Panel() = default;

    void Draw(Style style, Vector2 offset) override;
  };

  class Button : public Element {
  public:
    Button() = default;

    void Draw(Style style, Vector2 offset) override;
  };

  class Text : public Element {
  public:
    explicit Text(std::string text) : _text(std::move(text)) {}
    void Draw(Style style, Vector2 offset) override;
    [[nodiscard]] const std::string &GetText() const { return _text; }

  private:
    std::string _text;
  };


} // namespace slam::ui
