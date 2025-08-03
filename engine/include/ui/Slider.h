#pragma once
#include <functional>
#include "BasicElements.h"

namespace slam::ui {

class Slider : public Panel {
  public:
    Slider() = default;

    void OnCreate() override;
    void OnUpdate(float dt) override;

    void SetValue(float value);
    [[nodiscard]] float GetValue() const { return currentValue; }
    void SetRange(float min, float max);

    std::function<void(float)> OnValueChange;
    bool IsHovered = false;
    bool IsDragging = false;

  private:
    Panel* handle = nullptr;
    float minValue = 0.0f;
    float maxValue = 1.0f;
    float currentValue = 0.5f;
    const Color defaultHandleColor = ::DARKGRAY;
    const Color hoverHandleColor = { 75, 75, 75, 255};
    const Color sliderTrackColor = ::LIGHTGRAY;

    void updateTrackPosition() const;
    float calculateValueFromPosition(Vector2 mousePos) const;
  };


}
