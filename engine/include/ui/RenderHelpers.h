#pragma once
#include <string>
#include "ui/Style.h"
#include "raylib.h"
#include "Vector.h"

namespace slam::ui::RenderHelpers {
    void DrawStyledRectangle(const Style& style, const Vector2& offset);
    void DrawTextCentered(const std::string& text, const Style& style, const Vector2& offset);
    void DrawStyledText(const std::string& text, Style& style, const Vector2& offset);
}
