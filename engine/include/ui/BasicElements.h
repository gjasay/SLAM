#pragma once
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
    Text(const std::string &text) : _text(text) {}
    void Draw(Style style, Vector2 offset) override;
    const std::string &GetText() const { return _text; }

  private:
    std::string _text;
  };

  class Slider : public Panel {
  public:
    Slider() = default;

    void OnCreate() override {
      sliderTrack = static_cast<Panel*>(this->AddChild(std::make_unique<slam::ui::Panel>()));
      sliderTrack->inlineStyle->position.x = 600;
      sliderTrack->inlineStyle->widthIsPercent = true;
      sliderTrack->inlineStyle->heightIsPercent = true;
      sliderTrack->inlineStyle->widthPercent = 0.075f;
      sliderTrack->inlineStyle->heightPercent = 1.0f;
      sliderTrack->inlineStyle->backgroundColor = BLACK;
      this->inlineStyle->zIndex = 10;
    }

  private:
    Panel* sliderTrack = nullptr;
  };
} // namespace slam::ui
