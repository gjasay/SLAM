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

  class TextInput : public Panel {
  public:
    TextInput() {}
    void OnCreate() override;
    void OnUpdate(float dt) override;
    std::function<void(const std::string &)> OnTextChange;
    std::function<void()> OnSubmit;

  private:
    Text* textElement = nullptr;
    std::string content;
    float backspaceTimer = 0.0f;
    bool backspaceHeld = false;
    const float backspaceRepeatRate = 0.1f;
    float blinkTimer = 0.0f;
    bool blinkVisible = true;
    const float blinkInterval = 0.5f;
  };
} // namespace slam::ui
