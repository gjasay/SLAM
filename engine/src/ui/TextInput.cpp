#include "./ui/TextInput.h"
#include "ui/UIFactory.h"

namespace slam::ui {
  void KeyRepeatHandler::Update(float dt, bool keyDown, std::function<void()> action) {
    if (keyDown) {
      if (!isHeld) {
        action();
        isHeld = true;
        timer = 0.0f;
      } else {
        timer += dt;
        if (timer >= REPEAT_DELAY) {
          float repeatTime = timer - REPEAT_DELAY;
          if (repeatTime >= REPEAT_RATE) {
            action();
            timer = REPEAT_DELAY;
          }
        }
      }
    } else {
      Reset();
    }
  }

  void KeyRepeatHandler::Reset() {
    isHeld = false;
    timer = 0.0f;
  }

  void CursorBlinker::Update(float dt, bool isNavigating) {
    if (isNavigating) {
      isVisible = true;
      timer = 0.0f;
    } else {
      timer += dt;
      if (timer >= BLINK_INTERVAL) {
        isVisible = !isVisible;
        timer -= BLINK_INTERVAL;
      }
    }
  }

  void CursorBlinker::Reset() {
    timer = 0.0f;
    isVisible = false;
  }

  void TextViewport::UpdateScroll(float cursorPixelPos, float availableWidth) {
    if (cursorPixelPos - offsetX > availableWidth - SCROLL_MARGIN) {
      offsetX = cursorPixelPos - availableWidth + SCROLL_MARGIN;
    } else if (cursorPixelPos - offsetX < SCROLL_MARGIN) {
      offsetX = std::max(0.0f, cursorPixelPos - SCROLL_MARGIN);
    }
  }

  void TextInput::OnCreate() {
    SetupContainer();
    SetupTextElement();
    SetupCursorElement();
  }

  void TextInput::SetupContainer() {
    Factory::SetupInputContainer(this, 200, 40, 0.1f);
  }

  void TextInput::SetupTextElement() {
    textElement = Factory::AddText(this, "", 20, ::BLACK);
  }

  void TextInput::SetupCursorElement() {
    cursorElement = Factory::AddCursor(this, 20, ::BLACK);
  }

  void TextInput::OnUpdate(float dt) {
    SetMouseCursor(IsHovered ? ::MOUSE_CURSOR_IBEAM : ::MOUSE_CURSOR_DEFAULT);

    if (!IsFocused) {
      blinker.Reset();
      return;
    }

    HandleTextInput();
    HandleNavigation(dt);
    HandleEditing(dt);
    UpdateCursorAndText();
  }

  void TextInput::HandleTextInput() {
    const int key = ::GetCharPressed();
    if (key > 0) {
      InsertText(static_cast<char>(key));
    }

    if (::IsKeyPressed(::KEY_ENTER) && !content.empty()) {
      if (OnSubmit) OnSubmit();
    }
  }

  void TextInput::HandleNavigation(float dt) {
    const bool ctrlDown = ::IsKeyDown(::KEY_LEFT_CONTROL) || ::IsKeyDown(::KEY_RIGHT_CONTROL);
    bool isNavigating = false;

    if (::IsKeyPressed(::KEY_HOME)) {
      MoveCursor(0);
      isNavigating = true;
    }
    if (::IsKeyPressed(::KEY_END)) {
      MoveCursor(static_cast<int>(content.length()));
      isNavigating = true;
    }

    keyHandlers["ctrlLeft"].Update(dt, ctrlDown && ::IsKeyDown(::KEY_LEFT), [&]() {
      MoveCursor(FindPreviousWordBoundary(cursorPosition));
      isNavigating = true;
    });

    keyHandlers["ctrlRight"].Update(dt, ctrlDown && ::IsKeyDown(::KEY_RIGHT), [&]() {
      MoveCursor(FindNextWordBoundary(cursorPosition));
      isNavigating = true;
    });

    keyHandlers["left"].Update(dt, !ctrlDown && ::IsKeyDown(::KEY_LEFT), [&]() {
      MoveCursor(std::max(0, cursorPosition - 1));
      isNavigating = true;
    });

    keyHandlers["right"].Update(dt, !ctrlDown && ::IsKeyDown(::KEY_RIGHT), [&]() {
      MoveCursor(std::min(static_cast<int>(content.length()), cursorPosition + 1));
      isNavigating = true;
    });

    blinker.Update(dt, isNavigating);
  }

  void TextInput::HandleEditing(float dt) {
    const bool ctrlDown = ::IsKeyDown(::KEY_LEFT_CONTROL) || ::IsKeyDown(::KEY_RIGHT_CONTROL);

    keyHandlers["ctrlBackspace"].Update(dt, ctrlDown && ::IsKeyDown(::KEY_BACKSPACE), [&]() {
      DeletePreviousWord();
    });

    keyHandlers["backspace"].Update(dt, !ctrlDown && ::IsKeyDown(::KEY_BACKSPACE), [&]() {
      if (!content.empty() && cursorPosition > 0) {
        DeleteText(cursorPosition - 1, 1);
        cursorPosition--;
      }
    });

    keyHandlers["ctrlDelete"].Update(dt, ctrlDown && ::IsKeyDown(::KEY_DELETE), [&]() {
      DeleteNextWord();
    });

    keyHandlers["delete"].Update(dt, !ctrlDown && ::IsKeyDown(::KEY_DELETE), [&]() {
      if (cursorPosition < static_cast<int>(content.length())) {
        DeleteText(cursorPosition, 1);
      }
    });
  }

  void TextInput::UpdateCursorAndText() {
    if (!textElement || !cursorElement) return;

    std::string textToCursor = content.substr(0, cursorPosition);
    float cursorPixelPos = MeasureTextEx(::GetFontDefault(), textToCursor.c_str(),
                                        static_cast<float>(textElement->defaultStyle->fontSize), 1.0f).x;

    const int containerPadding = GetStyle().padding;
    auto availableWidth = static_cast<float>(defaultStyle->width - (containerPadding * 2));
    viewport.UpdateScroll(cursorPixelPos, availableWidth);

    textElement->InnerText = content;
    const int containerHeight = GetStyle().height;
    const int textHeight = textElement->defaultStyle->fontSize;
    const int verticalCenter = (containerHeight - textHeight) / 2;

    if (textElement->inlineStyle) {
      textElement->inlineStyle->position.x = containerPadding - static_cast<int>(viewport.GetOffsetX());
      textElement->inlineStyle->position.y = verticalCenter;
    }

    if (cursorElement->inlineStyle) {
      float exactCursorPos = cursorPixelPos - viewport.GetOffsetX();
      cursorElement->inlineStyle->position.x = containerPadding + static_cast<int>(exactCursorPos);
      cursorElement->inlineStyle->position.y = verticalCenter;
    }

    if (cursorElement->defaultStyle) {
      cursorElement->defaultStyle->visible = blinker.IsVisible();
    }
  }

  void TextInput::MoveCursor(int newPosition) {
    cursorPosition = std::max(0, std::min(static_cast<int>(content.length()), newPosition));
  }

  void TextInput::InsertText(char character) {
    content.insert(cursorPosition, 1, character);
    cursorPosition++;
    if (OnTextChange) OnTextChange(content);
  }

  void TextInput::DeleteText(int startPos, int length) {
    if (startPos >= 0 && startPos < static_cast<int>(content.length())) {
      content.erase(startPos, length);
      if (OnTextChange) OnTextChange(content);
    }
  }

  void TextInput::DeletePreviousWord() {
    int wordStart = FindPreviousWordBoundary(cursorPosition);
    if (wordStart < cursorPosition) {
      DeleteText(wordStart, cursorPosition - wordStart);
      cursorPosition = wordStart;
    }
  }

  void TextInput::DeleteNextWord() {
    int wordEnd = FindNextWordBoundary(cursorPosition);
    if (wordEnd > cursorPosition) {
      DeleteText(cursorPosition, wordEnd - cursorPosition);
    }
  }

  int TextInput::FindNextWordBoundary(int position) const {
    if (position >= static_cast<int>(content.length())) {
      return static_cast<int>(content.length());
    }

    while (position < static_cast<int>(content.length()) &&
           (std::isalnum(content[position]) || content[position] == '_')) {
      position++;
    }

    while (position < static_cast<int>(content.length()) &&
           !std::isalnum(content[position]) && content[position] != '_') {
      position++;
    }

    return position;
  }

  int TextInput::FindPreviousWordBoundary(int position) const {
    if (position <= 0) return 0;

    position--;

    while (position > 0 &&
           !std::isalnum(content[position]) && content[position] != '_') {
      position--;
    }

    while (position > 0 &&
           (std::isalnum(content[position]) || content[position] == '_')) {
      position--;
    }

    if (position > 0 || (!std::isalnum(content[0]) && content[0] != '_')) {
      position++;
    }

    return position;
  }
}