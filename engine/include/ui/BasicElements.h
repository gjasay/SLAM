#pragma once
#include <functional>
#include <utility>

#include "Element.h"

namespace slam::ui {
  class Panel : public Element {
  public:
    Panel() = default;

    void Draw(Style style, Vector2 offset) override;
  };

  class Button : public Panel {
  public:
    Button() = default;

    void OnUpdate(float dt) override;

    std::function<void()> OnClick;
    bool IsHovered = false;
    bool IsPressed = false;
  };

  class Text : public Element {
  public:
    explicit Text(std::string text) : InnerText(std::move(text)) {}
    void Draw(Style style, Vector2 offset) override;
    std::string InnerText;

  private:
  };


} // namespace slam::ui
