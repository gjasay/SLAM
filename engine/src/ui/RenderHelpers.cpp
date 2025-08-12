#include "ui/RenderHelpers.h"
#include "ui/Style.h"
#include "Vector.h"
#include "raylib.h"

namespace slam::ui::RenderHelpers {
    void DrawStyledRectangle(const Style& style, const Vector2& offset) {
        Rectangle rect{style.position.x + offset.x,
                       style.position.y + offset.y,
                       static_cast<float>(style.width),
                       static_cast<float>(style.height)};
        if (style.borderRadius > 0.0f) {
            constexpr int segments = 16;
            DrawRectangleRounded(rect, style.borderRadius, segments, style.backgroundColor);
            if (style.borderWidth > 0) {
                DrawRectangleRoundedLinesEx(rect, style.borderRadius, segments, style.borderWidth, style.borderColor);
            }
        } else {
            DrawRectangleRec(rect, style.backgroundColor);
            if (style.borderWidth > 0) {
                DrawRectangleLinesEx(rect, style.borderWidth, style.borderColor);
            }
        }
    }

    void DrawTextCentered(const std::string& text, const Style& style, const Vector2& offset) {
        if (text.empty()) return;
        Vector2 textSize = MeasureTextEx(::GetFontDefault(), text.c_str(), static_cast<float>(style.fontSize), 1.0f);
        float x = style.position.x + offset.x + (style.width - textSize.x) / 2.0f;
        float y = style.position.y + offset.y + (style.height - textSize.y) / 2.0f;
        DrawTextPro(::GetFontDefault(), text.c_str(), {x, y}, {0.0f, 0.0f}, 0.0f, style.fontSize, 1.0f, style.color);
    }
    void DrawStyledText(const std::string& text, Style& style, const Vector2& offset) {
        if (text.empty()) return;
        // Measure and update style size
        Vector2 textSize = MeasureTextEx(::GetFontDefault(), text.c_str(), static_cast<float>(style.fontSize), 1.0f);
        style.width = textSize.x;
        style.height = textSize.y;
        // Draw text at position
        DrawTextPro(::GetFontDefault(), text.c_str(), style.position + offset, {0.0f, 0.0f}, 0.0f,
                    style.fontSize, 1.0f, style.color);
        // Draw border if needed
        if (style.borderWidth > 0) {
            DrawRectangleLines(static_cast<int>(style.position.x + offset.x), static_cast<int>(style.position.y + offset.y),
                                static_cast<int>(style.width), static_cast<int>(style.height), style.borderColor);
        }
    }
}
