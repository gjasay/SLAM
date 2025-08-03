#include "../../include/ui/Slider.h"
#include "raylib.h"

#include "ui/Canvas.h"

namespace slam::ui {
  void Slider::OnCreate() {
    const auto finalStyle = this->canvas->styles.Resolve(this);
    handle = dynamic_cast<Panel *>(this->AddChild(std::make_unique<Panel>()));
    handle->defaultStyle->position.x = 0;
    handle->defaultStyle->widthIsPercent = true;
    handle->defaultStyle->heightIsPercent = true;
    handle->defaultStyle->widthPercent = 0.075f;
    handle->defaultStyle->heightPercent = 1.0f;
    handle->defaultStyle->borderColor = BLACK;
    handle->defaultStyle->borderWidth = 1;
    handle->defaultStyle->borderRadius = 0.75f;
    handle->defaultStyle->backgroundColor = defaultHandleColor;

    this->defaultStyle->backgroundColor = sliderTrackColor;
    this->defaultStyle->height = 20;
    this->defaultStyle->borderRadius = 0.35f;
    this->defaultStyle->borderColor = BLACK;
    this->defaultStyle->borderWidth = 1;
  }

void Slider::OnUpdate(float dt) {
    const auto finalStyle = this->GetStyle();
    const auto handleStyle = handle->GetStyle();
    const Vector2 mousePos = ::GetMousePosition();

    Rectangle bounds{finalStyle.position.x, finalStyle.position.y, finalStyle.width, finalStyle.height};
    bool isMouseOverSlider = CheckCollisionPointRec(mousePos, bounds);
    IsHovered = isMouseOverSlider;

    if (IsHovered) {
        handle->defaultStyle->backgroundColor = hoverHandleColor;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            IsDragging = true;
            SetValue(calculateValueFromPosition(mousePos));
        }
    } else {
        handle->defaultStyle->backgroundColor = defaultHandleColor;
    }

    if (IsDragging) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            // Account for handle width in bounds checking
            float minBound = finalStyle.position.x + (handleStyle.width / 2);
            float maxBound = finalStyle.position.x + finalStyle.width - (handleStyle.width / 2);

            // Only update if mouse is within adjusted bounds
            if (mousePos.x >= minBound && mousePos.x <= maxBound) {
                SetValue(calculateValueFromPosition(mousePos));
            }
        } else {
            IsDragging = false;
        }
    }
}

  void Slider::SetValue(float value) {
    float newValue = std::clamp(value, minValue, maxValue);
    if (newValue != currentValue) {
      currentValue = newValue;
      updateTrackPosition();

      if (OnValueChange) {
        OnValueChange(currentValue);
      }
    }
  }

  void Slider::SetRange(float min, float max) {
    if (min < max) {
      minValue = min;
      maxValue = max;
      SetValue(currentValue);
    }
  }

  float Slider::calculateValueFromPosition(const Vector2 mousePos) const {
    const auto sliderStyle = this->GetStyle();
    const auto handleStyle = handle->GetStyle();

    // Calculate relative position, keeping handle centered on mouse
    float relativeX = mousePos.x - sliderStyle.position.x - (handleStyle.width / 2);

    // Calculate percentage using available width
    float availableWidth = sliderStyle.width - handleStyle.width;
    float percentage = relativeX / availableWidth;
    percentage = std::clamp(percentage, 0.0f, 1.0f);

    return minValue + (percentage * (maxValue - minValue));
}

  void Slider::updateTrackPosition() const {
    if (!handle)
      return;

    const auto sliderStyle = this->GetStyle();
    const auto handleStyle = handle->GetStyle();

    float percent = (currentValue - minValue) / (maxValue - minValue);
    percent = std::clamp(percent, 0.0f, 1.0f);
    const float newX = percent * (sliderStyle.width - handleStyle.width);
    handle->inlineStyle->position.x = newX;
  }

} // namespace slam::ui
