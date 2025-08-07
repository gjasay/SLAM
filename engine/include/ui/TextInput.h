#pragma once
#include <string>
#include "ui/BasicElements.h"

namespace slam::ui {
  class TextInput : public Panel {
  public:
    TextInput() = default;
    void OnCreate() override;
    void OnUpdate(float dt) override;
    std::function<void(const std::string &)> OnTextChange;
    std::function<void()> OnSubmit;

  private:
    Text* textElement = nullptr;
    Text* cursorElement = nullptr;
    std::string content;
    int cursorPosition = 0;
    float textOffsetX = 0.0f;

    float backspaceTimer = 0.0f;
    bool backspaceHeld = false;
    float deleteTimer = 0.0f;
    bool deleteHeld = false;
    float leftArrowTimer = 0.0f;
    bool leftArrowHeld = false;
    float rightArrowTimer = 0.0f;
    bool rightArrowHeld = false;
    float ctrlLeftTimer = 0.0f;
    bool ctrlLeftHeld = false;
    float ctrlRightTimer = 0.0f;
    bool ctrlRightHeld = false;
    float ctrlBackspaceTimer = 0.0f;
    bool ctrlBackspaceHeld = false;
    float ctrlDeleteTimer = 0.0f;
    bool ctrlDeleteHeld = false;

    const float keyRepeatDelay = 0.5f;
    const float keyRepeatRate = 0.05f;

    float blinkTimer = 0.0f;
    bool blinkVisible = true;
    const float blinkInterval = 0.5f;

    int findNextWordBoundary(int position);
    int findPreviousWordBoundary(int position);
  };

}
