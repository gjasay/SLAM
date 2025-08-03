#include "../../include/ui/Element.h"
#include "../../include/ui/Canvas.h"
#include "../../include/ui/Style.h"
#include "raylib.h"
#include "ui/BasicElements.h"

namespace slam::ui {


  void Element::_draw(const Vector2 offset) {
    Style finalStyle = canvas->styles.Resolve(this);

    resolvePercentSizes(finalStyle);

    if (finalStyle.flex && !children.empty()) {
      applyFlexLayout(finalStyle);
    }

    renderElements(finalStyle, offset);
  }

  void Element::resolvePercentSizes(Style &finalStyle) {
    auto resolvePercent = [](Style &style, Element *elem, int parentW, int parentH) {
      if (style.widthIsPercent) {
        style.width = static_cast<int>(parentW * style.widthPercent);
        if (elem && elem->inlineStyle)
          elem->inlineStyle->width = style.width;
      }
      if (style.heightIsPercent) {
        style.height = static_cast<int>(parentH * style.heightPercent);
        if (elem && elem->inlineStyle)
          elem->inlineStyle->height = style.height;
      }
    };

    // Resolve percent sizes for children
    for (const auto &child: children) {
      Style childStyle = canvas->styles.Resolve(child.get());
      resolvePercent(childStyle, child.get(), finalStyle.width, finalStyle.height);
    }
  }

  void Element::applyFlexLayout(Style &finalStyle) {
    bool isRow = finalStyle.flexDirection == FlexDirection::Row;

    int totalChildSize = 0;
    int containerSize = 0;
    int remainingSpace = 0;

    calculateFlexMeasurements(finalStyle, isRow, totalChildSize, containerSize, remainingSpace);

    float mainAxisOffset = finalStyle.padding;
    calculateFlexOffsets(finalStyle, remainingSpace, mainAxisOffset);

    layoutFlexChildren(finalStyle, isRow, mainAxisOffset);
  }

  void Element::calculateFlexMeasurements(Style &finalStyle, bool isRow, int &totalChildSize, int &containerSize,
                                          int &remainingSpace) {
    totalChildSize = 0;

    // Measure each child's size
    for (const auto &child: children) {
      Style childStyle = canvas->styles.Resolve(child.get());
      if (auto textChild = dynamic_cast<Text *>(child.get())) {
        const auto size = MeasureTextEx(::GetFontDefault(), textChild->GetText().c_str(), childStyle.fontSize, 1.0f);
        childStyle.width = static_cast<int>(size.x);
        childStyle.height = static_cast<int>(size.y);
      }
      totalChildSize += isRow ? childStyle.width : childStyle.height;
    }

    int totalGap = finalStyle.flexGap * (children.size() - 1);
    containerSize = isRow ? finalStyle.width - 2 * finalStyle.padding : finalStyle.height - 2 * finalStyle.padding;

    int contentSize = totalChildSize + totalGap;
    remainingSpace = containerSize - contentSize;
  }

  void Element::calculateFlexOffsets(Style &finalStyle, int remainingSpace, float &mainAxisOffset) {
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
      case JustifyContent::SpaceAround:
        if (!children.empty()) {
          float gap = remainingSpace / static_cast<float>(children.size());
          finalStyle.flexGap = static_cast<int>(gap);
          mainAxisOffset += gap * 0.5f;
        }
        break;
      case JustifyContent::SpaceEvenly:
        if (!children.empty()) {
          float gap = remainingSpace / static_cast<float>(children.size() + 1);
          finalStyle.flexGap = static_cast<int>(gap);
          mainAxisOffset += gap;
        }
        break;
      default:
        break;
    }
  }

  void Element::layoutFlexChildren(Style &finalStyle, bool isRow, float mainAxisOffset) {
    for (auto &child: children) {
      Style childStyle = canvas->styles.Resolve(child.get());
      if (auto textChild = dynamic_cast<Text *>(child.get())) {
        const auto size = MeasureTextEx(::GetFontDefault(), textChild->GetText().c_str(), childStyle.fontSize, 1.0f);
        childStyle.width = static_cast<int>(size.x);
        childStyle.height = static_cast<int>(size.y);
      }

      float crossAxisOffset = finalStyle.padding;
      if (isRow) {
        int extra = finalStyle.height - 2 * finalStyle.padding - childStyle.height;
        if (finalStyle.alignItems == AlignItems::Center)
          crossAxisOffset += extra / 2.0f;
        else if (finalStyle.alignItems == AlignItems::FlexEnd)
          crossAxisOffset += extra;
        // Baseline and default: do nothing
        childStyle.position = {mainAxisOffset, crossAxisOffset};
        mainAxisOffset += childStyle.width + finalStyle.flexGap;
      } else {
        int extra = finalStyle.width - 2 * finalStyle.padding - childStyle.width;
        if (finalStyle.alignItems == AlignItems::Center)
          crossAxisOffset += extra / 2.0f;
        else if (finalStyle.alignItems == AlignItems::FlexEnd)
          crossAxisOffset += extra;
        childStyle.position = {crossAxisOffset, mainAxisOffset};
        mainAxisOffset += childStyle.height + finalStyle.flexGap;
      }

      if (child->inlineStyle)
        child->inlineStyle->position = childStyle.position;
    }
  }

  void Element::renderElements(const Style &finalStyle, const Vector2 &offset) {
    // Draw self and children
    if (finalStyle.visible && this->id != "canvas")
      Draw(finalStyle, offset);

    // Create a vector of pairs containing child elements and their z-index
    std::vector<std::pair<Element *, int>> sortedChildren;
    for (const auto &child: children) {
      Style childStyle = canvas->styles.Resolve(child.get());
      sortedChildren.emplace_back(child.get(), childStyle.zIndex);
    }

    // Sort children by z-index
    std::sort(sortedChildren.begin(), sortedChildren.end(),
              [](const auto &a, const auto &b) { return a.second < b.second; });

    // Draw children in sorted order
    for (const auto &[child, _]: sortedChildren) {
      child->_draw(offset + finalStyle.position);
    }
  }


} // namespace slam::ui
