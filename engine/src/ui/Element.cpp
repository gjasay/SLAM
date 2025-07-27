#include "../../include/ui/Element.h"
#include "../../include/ui/Canvas.h"

namespace slam::ui {
  void Element::_draw(const Vector2 offset) {
    const Style finalStyle = canvas->styles.Resolve(this);

    // If flex is enabled and children exist, compute layout with justifyContent and alignItems.
    if (finalStyle.flex && !children.empty()) {
      // Determine main axis
      bool isRow = finalStyle.flexDirection == FlexDirection::Row;

      // Sum dimensions and count gaps.
      int totalChildSize = 0;
      for (const auto &child: children) {
        totalChildSize += isRow ? child->width : child->height;
      }
      int totalGap = finalStyle.flexGap * (children.size() - 1);
      int containerSize = isRow ? this->width - 2 * finalStyle.padding : this->height - 2 * finalStyle.padding;
      int contentSize = totalChildSize + totalGap;
      int remainingSpace = containerSize - contentSize;

      // Compute offset for justifyContent on main axis.
      float mainAxisOffset = finalStyle.padding;
      switch (finalStyle.justifyContent) {
        case JustifyContent::Center:
          mainAxisOffset += remainingSpace / 2.0f;
          break;
        case JustifyContent::FlexEnd:
          mainAxisOffset += remainingSpace;
          break;
        case JustifyContent::SpaceBetween:
          // Overwrite flexGap if more than 1 child.
          if (children.size() > 1)
            finalStyle.flexGap = remainingSpace / (children.size() - 1);
          break;
        default:
          break;
      }

      // Position each child.
      for (auto &child: children) {
        // Compute cross axis offset for alignItems.
        float crossAxisOffset = finalStyle.padding;
        if (isRow) {
          int extra = this->height - 2 * finalStyle.padding - child->height;
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
          child->position = {mainAxisOffset, crossAxisOffset};
          mainAxisOffset += child->width + finalStyle.flexGap;
        } else {
          int extra = this->width - 2 * finalStyle.padding - child->width;
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
          child->position = {crossAxisOffset, mainAxisOffset};
          mainAxisOffset += child->height + finalStyle.flexGap;
        }
      }
    }

    // Draw the element.
    Draw(finalStyle, offset);

    // Draw children.
    for (const auto &child: children) {
      child->_draw(offset + position);
    }
  }

  void Panel::Draw(const Style style, const Vector2 offset) {
    const Rectangle rect = {position.x + offset.x, position.y + offset.y, static_cast<float>(width),
                            static_cast<float>(height)};

    if (style.borderRadius > 0.0f) {
      DrawRectangleRounded(rect, style.borderRadius, 0, style.backgroundColor);
      if (style.borderWidth > 0) {
        DrawRectangleRoundedLines(rect, style.borderRadius, style.borderWidth, style.borderColor);
      }
    } else {
      DrawRectangleRec(rect, style.backgroundColor);
      if (style.borderWidth > 0) {
        DrawRectangleLinesEx(rect, style.borderWidth, style.borderColor);
      }
    }
  }

  void Text::Draw(const Style style, const Vector2 offset) {
    const auto size = MeasureTextEx(::GetFontDefault(), _text.c_str(), style.fontSize, 1.0f);
    width = static_cast<int>(size.x);
    height = static_cast<int>(size.y);
    DrawTextEx(::GetFontDefault(), _text.c_str(), position + offset, style.fontSize, 1.0f, style.color);
    if (style.borderWidth > 0) {
      DrawRectangleLines(position.x + offset.x, position.y + offset.y, width, height, style.borderColor);
    }
  }


} // namespace slam::ui
