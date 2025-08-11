#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include "ui/BasicElements.h"

namespace slam::ui {
  class KeyRepeatHandler {
  public:
    void Update(float dt, bool keyDown, std::function<void()> action);
    void Reset();

  private:
    bool isHeld = false;
    float timer = 0.0f;
    static constexpr float REPEAT_DELAY = 0.5f;
    static constexpr float REPEAT_RATE = 0.05f;
  };

  class CursorBlinker {
  public:
    void Update(float dt, bool isNavigating);
    bool IsVisible() const { return isVisible; }
    void Reset();

  private:
    float timer = 0.0f;
    bool isVisible = true;
    static constexpr float BLINK_INTERVAL = 0.5f;
  };

  class TextViewport {
  public:
    void UpdateScroll(float cursorPixelPos, float availableWidth);
    float GetOffsetX() const { return offsetX; }
    void Reset() { offsetX = 0.0f; }

  private:
    float offsetX = 0.0f;
    static constexpr float SCROLL_MARGIN = 20.0f;
  };

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

    TextViewport viewport;
    CursorBlinker blinker;
    std::unordered_map<std::string, KeyRepeatHandler> keyHandlers;

    void HandleTextInput();
    void HandleNavigation(float dt);
    void HandleEditing(float dt);
    void UpdateCursorAndText();

    int FindNextWordBoundary(int position) const;
    int FindPreviousWordBoundary(int position) const;
    void MoveCursor(int newPosition);

    void InsertText(char character);
    void DeleteText(int startPos, int length);
    void DeletePreviousWord();
    void DeleteNextWord();

    void SetupTextElement();
    void SetupCursorElement();
    void SetupContainer() const;
  };
}
