#include "../../include/ui/Element.h"
#include "../../include/ui/Canvas.h"
#include "../../include/ui/Style.h"
#include "raylib.h"

namespace slam::ui {

  void Element::_draw(const Vector2 offset) {
    Style finalStyle = canvas->styles.Resolve(this);

    if (finalStyle.flex && !children.empty()) {
      bool isRow = finalStyle.flexDirection == FlexDirection::Row;
      int totalChildSize = 0;
      // Loop to measure each child's size
      for (const auto &child: children) {
        Style childStyle = canvas->styles.Resolve(child.get());
        // If child is a Text element, update its size using MeasureTextEx.
        if (auto textChild = dynamic_cast<Text *>(child.get())) {
          const auto size = MeasureTextEx(::GetFontDefault(), textChild->GetText().c_str(), childStyle.fontSize, 1.0f);
          childStyle.width = static_cast<int>(size.x);
          childStyle.height = static_cast<int>(size.y);
        }
        totalChildSize += isRow ? childStyle.width : childStyle.height;
      }
      int totalGap = finalStyle.flexGap * (children.size() - 1);
      int containerSize =
          isRow ? finalStyle.width - 2 * finalStyle.padding : finalStyle.height - 2 * finalStyle.padding;
      int contentSize = totalChildSize + totalGap;
      int remainingSpace = containerSize - contentSize;

      float mainAxisOffset = finalStyle.padding;
      switch (finalStyle.justifyContent) {
        case JustifyContent::Center:
          mainAxisOffset += remainingSpace / 2.0f;
          break;
        case JustifyContent::FlexEnd:
          mainAxisOffset += remainingSpace;
          break;
        case JustifyContent::SpaceBetween:
          if (children.size() > 1)
            finalStyle.flexGap = remainingSpace / (children.size() - 1);
          break;
        default:
          break;
      }

      for (auto &child: children) {
        Style childStyle = canvas->styles.Resolve(child.get());
        // If child is a text element, re-measure its size
        if (auto textChild = dynamic_cast<Text *>(child.get())) {
          const auto size = MeasureTextEx(::GetFontDefault(), textChild->GetText().c_str(), childStyle.fontSize, 1.0f);
          childStyle.width = static_cast<int>(size.x);
          childStyle.height = static_cast<int>(size.y);
        }
        float crossAxisOffset = finalStyle.padding;
        if (isRow) {
          int extra = finalStyle.height - 2 * finalStyle.padding - childStyle.height;
          switch (finalStyle.alignItems) {
            case AlignItems::Center:
              crossAxisOffset += extra / 2.0f;
              break;
            case AlignItems::FlexEnd:
              crossAxisOffset += extra;
              break;
            default:
              break;
          }
          childStyle.position = {mainAxisOffset, crossAxisOffset};
          mainAxisOffset += childStyle.width + finalStyle.flexGap;
        } else {
          int extra = finalStyle.width - 2 * finalStyle.padding - childStyle.width;
          switch (finalStyle.alignItems) {
            case AlignItems::Center:
              crossAxisOffset += extra / 2.0f;
              break;
            case AlignItems::FlexEnd:
              crossAxisOffset += extra;
              break;
            default:
              break;
          }
          childStyle.position = {crossAxisOffset, mainAxisOffset};
          mainAxisOffset += childStyle.height + finalStyle.flexGap;
        }
        if (child->inlineStyle) {
          child->inlineStyle->position = childStyle.position;
        }
      }
    }

    if (finalStyle.visible)
      Draw(finalStyle, offset);
    for (const auto &child: children) {
      child->_draw(offset + finalStyle.position);
    }
  }

  void Panel::Draw(const Style style, const Vector2 offset) {
    const Rectangle rect = {style.position.x + offset.x, style.position.y + offset.y, static_cast<float>(style.width),
                            static_cast<float>(style.height)};
    if (style.borderRadius > 0.0f) {
      DrawRectangleRounded(rect, style.borderRadius, 0, style.backgroundColor);
      if (style.borderWidth > 0)
        DrawRectangleRoundedLines(rect, style.borderRadius, style.borderWidth, style.borderColor);
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

} // namespace slam::ui
