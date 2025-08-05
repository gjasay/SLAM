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
} // namespace slam::ui
