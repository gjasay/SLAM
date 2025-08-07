#pragma once
#include <functional>
#include <memory>
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
    explicit Button(std::string text) : InnerText(std::move(text)) {}
    void OnUpdate(float dt) override;
    void Draw(Style style, Vector2 offset) override;
    std::function<void()> OnClick;
    std::string InnerText;
  };

  class Text : public Element {
  public:
    explicit Text(std::string text) : InnerText(std::move(text)) {}
    void Draw(Style style, Vector2 offset) override;
    std::string InnerText;
  };


} // namespace slam::ui
