#include "ui/BasicElements.h"
#include "ui/RenderHelpers.h"
#include "raylib.h"

namespace slam::ui {
  void Panel::Draw(const Style style, const Vector2 offset) {
    RenderHelpers::DrawStyledRectangle(style, offset);
  }

  void Button::OnUpdate(float dt) {
    if (IsActive && IsFocused && OnClick) {
      OnClick();
    }
  }

  void Button::Draw(const Style style, const Vector2 offset) {
    RenderHelpers::DrawStyledRectangle(style, offset);
    RenderHelpers::DrawTextCentered(InnerText, style, offset);
  }

  void Text::Draw(Style style, const Vector2 offset) {
    const auto size = MeasureTextEx(::GetFontDefault(), InnerText.c_str(), style.fontSize, 1.0f);
    style.width = static_cast<int>(size.x);
    style.height = static_cast<int>(size.y);
    DrawTextPro(::GetFontDefault(), InnerText.c_str(), style.position + offset, {0.0f, 0.0f}, 0.0f, style.fontSize, 1.0f, style.color);
    if (style.borderWidth > 0)
      DrawRectangleLines(style.position.x + offset.x, style.position.y + offset.y, style.width, style.height,
                         style.borderColor);
  }


} // namespace slam::ui
