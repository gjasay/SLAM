#include <iostream>
#include "../../include/ui/Element.h"
#include "../../include/ui/Canvas.h"
#include "../../include/ui/StyleManager.h"
#include "raylib.h"
#include "ui/BasicElements.h"

namespace slam::ui {
  void Element::_draw(const Vector2 offset) {
    Style layoutStyle = canvas->styles.Resolve(this, StyleState::Default);
    resolvePercentSizes(layoutStyle);
    if (layoutStyle.flex && !children.empty()) {
      applyFlexLayout(layoutStyle);
    }

    Vector2 absPos{layoutStyle.position.x + offset.x, layoutStyle.position.y + offset.y};
    const auto mousePos = ::GetMousePosition();
    Rectangle bounds{absPos.x, absPos.y, static_cast<float>(layoutStyle.width), static_cast<float>(layoutStyle.height)};
    bool hovered = CheckCollisionPointRec(mousePos, bounds);
    IsHovered = hovered;

    if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      IsActive = true;
      IsFocused = true;
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
      IsActive = false;
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !hovered) {
      IsFocused = false;
    }

    StyleState currentState = GetCurrentState();
    this->resolvedStyle = canvas->styles.Resolve(this, currentState);
    resolvePercentSizes(resolvedStyle);

    if (resolvedStyle.flex && !children.empty()) {
      applyFlexLayout(resolvedStyle);
    }
    renderElements(resolvedStyle, offset);
  }

  void Element::resolvePercentSizes(const Style &finalStyle) const {
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

    for (const auto &child: children) {
      Style childStyle = canvas->styles.Resolve(child.get());
      resolvePercent(childStyle, child.get(), finalStyle.width, finalStyle.height);
    }
  }

  void Element::applyFlexLayout(Style &finalStyle) {
    bool isRow = finalStyle.flexDirection == FlexDirection::Row;
    if (isRow && finalStyle.height == 0) {
      float maxH = 0.0f;
      for (auto &child: children) {
        Style childStyle = canvas->styles.Resolve(child.get());
        if (auto textChild = dynamic_cast<Text *>(child.get())) {
          const auto size = MeasureTextEx(::GetFontDefault(), textChild->InnerText.c_str(), childStyle.fontSize, 1.f);
          childStyle.width = static_cast<int>(size.x);
          childStyle.height = static_cast<int>(size.y);
        }
        maxH = std::max(maxH, childStyle.height);
      }
      finalStyle.height = maxH + 2 * finalStyle.padding;
      if (inlineStyle)
        inlineStyle->height = finalStyle.height;
    } else if (!isRow && finalStyle.width == 0) {
      float maxW = 0.0f;
      for (auto &child: children) {
        Style childStyle = canvas->styles.Resolve(child.get());
        if (auto textChild = dynamic_cast<Text *>(child.get())) {
          const auto size = MeasureTextEx(::GetFontDefault(), textChild->InnerText.c_str(), childStyle.fontSize, 1.f);
          childStyle.width = static_cast<int>(size.x);
          childStyle.height = static_cast<int>(size.y);
        }
        maxW = std::max(maxW, childStyle.width);
      }
      finalStyle.width = maxW + 2 * finalStyle.padding;
      if (inlineStyle)
        inlineStyle->width = finalStyle.width;
    }
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

    for (const auto &child: children) {
      Style childStyle = canvas->styles.Resolve(child.get());
      if (auto textChild = dynamic_cast<Text *>(child.get())) {
        const auto size = MeasureTextEx(::GetFontDefault(), textChild->InnerText.c_str(), childStyle.fontSize, 1.0f);
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
        const auto size = MeasureTextEx(::GetFontDefault(), textChild->InnerText.c_str(), childStyle.fontSize, 1.0f);
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
    if (finalStyle.visible && this->id != "canvas")
      Draw(finalStyle, offset);

    std::vector<std::pair<Element *, int>> sortedChildren;
    for (const auto &child: children) {
      Style childStyle = canvas->styles.Resolve(child.get());
      sortedChildren.emplace_back(child.get(), childStyle.zIndex);
    }

    std::sort(sortedChildren.begin(), sortedChildren.end(),
              [](const auto &a, const auto &b) { return a.second < b.second; });

    for (const auto &[child, _]: sortedChildren) {
      child->_draw(offset + finalStyle.position);
    }
  }

  void Element::_update(const float dt, const Vector2 offset) {
    if (IsDisabled) {
      IsHovered = false;
      IsActive = false;
    } else {
      const auto style = this->GetStyle();
      const auto mousePos = ::GetMousePosition();
      Vector2 absPos{style.position.x + offset.x, style.position.y + offset.y};
      Rectangle bounds{absPos.x, absPos.y, static_cast<float>(style.width), static_cast<float>(style.height)};

      AbsPosition = absPos;
      bool hovered = CheckCollisionPointRec(mousePos, bounds);
      if (hovered && !IsHovered) {
        IsHovered = true;
      } else if (!hovered) {
        IsHovered = false;
      }
      if (IsHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        IsActive = true;
        IsFocused = true;
      }
      if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        IsActive = false;
      }
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !IsHovered) {
        IsFocused = false;
      }
    }

    this->OnUpdate(dt);

    for (const auto &child: children) {
      const auto style = this->GetStyle();
      child->_update(dt, offset + style.position);
    }
  }

} // namespace slam::ui
