#include "ui/BasicElements.h"
#include "raylib.h"

namespace slam::ui {
  void Panel::Draw(const Style style, const Vector2 offset) {
    const Rectangle rect = {style.position.x + offset.x, style.position.y + offset.y, static_cast<float>(style.width),
                            static_cast<float>(style.height)};
    if (style.borderRadius > 0.0f) {
      constexpr int segments = 16;
      DrawRectangleRounded(rect, style.borderRadius, segments, style.backgroundColor);
      if (style.borderWidth > 0)
        DrawRectangleRoundedLinesEx(rect, style.borderRadius, segments, style.borderWidth, style.borderColor);
    } else {
      DrawRectangleRec(rect, style.backgroundColor);
      if (style.borderWidth > 0)
        DrawRectangleLinesEx(rect, style.borderWidth, style.borderColor);
    }
  }

  void Button::OnUpdate(float dt) {
    if (IsActive && IsFocused && OnClick) {
      OnClick();
    }
  }

  void Button::Draw(const Style style, const Vector2 offset) {
    const Rectangle rect = {style.position.x + offset.x, style.position.y + offset.y, static_cast<float>(style.width),
                            static_cast<float>(style.height)};

    DrawRectangleRec(rect, style.backgroundColor);
    if (style.borderWidth > 0) {
      DrawRectangleLinesEx(rect, style.borderWidth, style.borderColor);
    }

    if (!InnerText.empty()) {
      const auto textSize = MeasureTextEx(::GetFontDefault(), InnerText.c_str(), style.fontSize, 1.0f);
      Vector2 textPos = {rect.x + (rect.width - textSize.x) / 2, rect.y + (rect.height - textSize.y) / 2};
      DrawTextEx(::GetFontDefault(), InnerText.c_str(), textPos, style.fontSize, 1.0f, style.color);
    }
  }

  void Text::Draw(Style style, const Vector2 offset) {
    const auto size = MeasureTextEx(::GetFontDefault(), InnerText.c_str(), style.fontSize, 1.0f);
    style.width = static_cast<int>(size.x);
    style.height = static_cast<int>(size.y);
    DrawTextEx(::GetFontDefault(), InnerText.c_str(), style.position + offset, style.fontSize, 1.0f, style.color);
    if (style.borderWidth > 0)
      DrawRectangleLines(style.position.x + offset.x, style.position.y + offset.y, style.width, style.height,
                         style.borderColor);
  }

  void TextInput::OnCreate() {
    textElement = dynamic_cast<Text *>(this->AddChild(std::make_unique<Text>("")));

    if (textElement) {
      textElement->defaultStyle->fontSize = 20;
      textElement->defaultStyle->color = ::BLACK;
      textElement->defaultStyle->padding = 10;
      textElement->defaultStyle->marginLeft = 10;
    }
    this->defaultStyle->borderColor = ::GRAY;
    this->defaultStyle->borderWidth = 1;
    this->defaultStyle->borderRadius = 0.1f;
    this->defaultStyle->width = 200;
    this->defaultStyle->height = 40;
    this->defaultStyle->flex = true;
    this->defaultStyle->justifyContent = JustifyContent::FlexStart;
    this->defaultStyle->alignItems = AlignItems::Center;
  }

  void TextInput::OnUpdate(float dt) {
    if (!IsFocused) {
      return;
    }
    bool backspaceDown = ::IsKeyDown(::KEY_BACKSPACE);
    if (backspaceDown) {
      if (!backspaceHeld) {
        // initial delete
        if (!content.empty()) {
          content.pop_back();
          if (OnTextChange)
            OnTextChange(content);
        }
        backspaceHeld = true;
        backspaceTimer = 0.0f;
      } else {
        backspaceTimer += dt;
        if (backspaceTimer >= backspaceRepeatRate) {
          if (!content.empty()) {
            content.pop_back();
            if (OnTextChange)
              OnTextChange(content);
          }
          backspaceTimer -= backspaceRepeatRate;
        }
      }
    } else {
      backspaceHeld = false;
      backspaceTimer = 0.0f;
    }
    if (::IsKeyPressed(::KEY_ENTER) && !content.empty()) {
      if (OnSubmit) {
        OnSubmit();
      }
    }
    const int key = ::GetCharPressed();
    if (key > 0) {
      content += static_cast<char>(key);
      if (OnTextChange)
        OnTextChange(content);
    }
    if (IsFocused) {
      blinkTimer += dt;
      if (blinkTimer >= blinkInterval) {
        blinkVisible = !blinkVisible;
        blinkTimer -= blinkInterval;
      }
    } else {
      blinkVisible = false;
      blinkTimer = 0.0f;
    }
    if (textElement) {
      textElement->InnerText = content + (IsFocused && blinkVisible ? std::string("_") : std::string(""));
    }
  }

} // namespace slam::ui
