#include "../../include/ui/Slider.h"
#include "raylib.h"

#include "ui/Canvas.h"

namespace slam::ui {
  void Slider::OnCreate() {
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
            float minBound = AbsPosition.x + (handleStyle.width / 2);
            float maxBound = AbsPosition.x + finalStyle.width - (handleStyle.width / 2);

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

    float relativeX = mousePos.x - AbsPosition.x - (handleStyle.width / 2);

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
