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

  void Text::Draw(Style style, const Vector2 offset) {
    const auto size = MeasureTextEx(::GetFontDefault(), _text.c_str(), style.fontSize, 1.0f);
    style.width = static_cast<int>(size.x);
    style.height = static_cast<int>(size.y);
    DrawTextEx(::GetFontDefault(), _text.c_str(), style.position + offset, style.fontSize, 1.0f, style.color);
    if (style.borderWidth > 0)
      DrawRectangleLines(style.position.x + offset.x, style.position.y + offset.y, style.width, style.height,
                         style.borderColor);
  }
}
