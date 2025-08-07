#include "./ui/TextInput.h"

namespace slam::ui {
  void TextInput::OnCreate() {
    textElement = dynamic_cast<Text *>(this->AddChild(std::make_unique<Text>("")));
    cursorElement = dynamic_cast<Text *>(this->AddChild(std::make_unique<Text>("|")));

    if (textElement) {
      textElement->defaultStyle->fontSize = 20;
      textElement->defaultStyle->color = ::BLACK;
      textElement->defaultStyle->padding = 0;
      textElement->defaultStyle->marginLeft = 0;
      textElement->defaultStyle->absolutePosition = true; // Make text absolutely positioned too
    }

    if (cursorElement) {
      cursorElement->defaultStyle->fontSize = 20;
      cursorElement->defaultStyle->color = ::BLACK;
      cursorElement->defaultStyle->padding = 0;
      cursorElement->defaultStyle->marginLeft = 0;
      cursorElement->defaultStyle->zIndex = 1;
      cursorElement->defaultStyle->absolutePosition = true;
    }

    this->defaultStyle->borderColor = ::GRAY;
    this->defaultStyle->borderWidth = 1;
    this->defaultStyle->borderRadius = 0.1f;
    this->defaultStyle->width = 200;
    this->defaultStyle->height = 40;
    this->defaultStyle->padding = 10;
    this->defaultStyle->flex = false;
    this->defaultStyle->overflowHidden = true;
  }

  void TextInput::OnUpdate(float dt) {
    if (IsHovered) {
      SetMouseCursor(::MOUSE_CURSOR_IBEAM);
    } else {
      SetMouseCursor(::MOUSE_CURSOR_DEFAULT);
    }

    if (!IsFocused) {
      blinkVisible = false;
      blinkTimer = 0.0f;
      return;
    }

    bool isNavigating = false;
    const bool ctrlDown = ::IsKeyDown(::KEY_LEFT_CONTROL) || ::IsKeyDown(::KEY_RIGHT_CONTROL);

    // Ctrl + Left Arrow (jump to previous word)
    const bool ctrlLeftDown = ctrlDown && ::IsKeyDown(::KEY_LEFT);
    if (ctrlLeftDown) {
      isNavigating = true;
      if (!ctrlLeftHeld) {
        cursorPosition = findPreviousWordBoundary(cursorPosition);
        ctrlLeftHeld = true;
        ctrlLeftTimer = 0.0f;
      } else {
        ctrlLeftTimer += dt;
        if (ctrlLeftTimer >= keyRepeatDelay) {
          // After initial delay, repeat at faster rate
          float repeatTime = ctrlLeftTimer - keyRepeatDelay;
          if (repeatTime >= keyRepeatRate) {
            cursorPosition = findPreviousWordBoundary(cursorPosition);
            ctrlLeftTimer = keyRepeatDelay; // Reset to just past delay
          }
        }
      }
    } else {
      ctrlLeftHeld = false;
      ctrlLeftTimer = 0.0f;
    }

    const bool ctrlRightDown = ctrlDown && ::IsKeyDown(::KEY_RIGHT);
    if (ctrlRightDown) {
      isNavigating = true;
      if (!ctrlRightHeld) {
        cursorPosition = findNextWordBoundary(cursorPosition);
        ctrlRightHeld = true;
        ctrlRightTimer = 0.0f;
      } else {
        ctrlRightTimer += dt;
        if (ctrlRightTimer >= keyRepeatDelay) {
          float repeatTime = ctrlRightTimer - keyRepeatDelay;
          if (repeatTime >= keyRepeatRate) {
            cursorPosition = findNextWordBoundary(cursorPosition);
            ctrlRightTimer = keyRepeatDelay;
          }
        }
      }
    } else {
      ctrlRightHeld = false;
      ctrlRightTimer = 0.0f;
    }

    const bool leftArrowDown = !ctrlDown && ::IsKeyDown(::KEY_LEFT);
    if (leftArrowDown) {
      isNavigating = true;
      if (!leftArrowHeld) {
        cursorPosition = std::max(0, cursorPosition - 1);
        leftArrowHeld = true;
        leftArrowTimer = 0.0f;
      } else {
        leftArrowTimer += dt;
        if (leftArrowTimer >= keyRepeatDelay) {
          float repeatTime = leftArrowTimer - keyRepeatDelay;
          if (repeatTime >= keyRepeatRate) {
            cursorPosition = std::max(0, cursorPosition - 1);
            leftArrowTimer = keyRepeatDelay;
          }
        }
      }
    } else {
      leftArrowHeld = false;
      leftArrowTimer = 0.0f;
    }

    // Regular Right Arrow (only if Ctrl is not pressed)
    const bool rightArrowDown = !ctrlDown && ::IsKeyDown(::KEY_RIGHT);
    if (rightArrowDown) {
      isNavigating = true;
      if (!rightArrowHeld) {
        cursorPosition = std::min(static_cast<int>(content.length()), cursorPosition + 1);
        rightArrowHeld = true;
        rightArrowTimer = 0.0f;
      } else {
        rightArrowTimer += dt;
        if (rightArrowTimer >= keyRepeatDelay) {
          float repeatTime = rightArrowTimer - keyRepeatDelay;
          if (repeatTime >= keyRepeatRate) {
            cursorPosition = std::min(static_cast<int>(content.length()), cursorPosition + 1);
            rightArrowTimer = keyRepeatDelay;
          }
        }
      }
    } else {
      rightArrowHeld = false;
      rightArrowTimer = 0.0f;
    }

    if (::IsKeyPressed(::KEY_HOME)) {
      isNavigating = true;
      cursorPosition = 0;
    }

    if (::IsKeyPressed(::KEY_END)) {
      isNavigating = true;
      cursorPosition = static_cast<int>(content.length());
    }

    // Ctrl + Backspace (delete previous word)
    const bool ctrlBackspaceDown = ctrlDown && ::IsKeyDown(::KEY_BACKSPACE);
    if (ctrlBackspaceDown) {
      if (!ctrlBackspaceHeld) {
        int wordStart = findPreviousWordBoundary(cursorPosition);
        if (wordStart < cursorPosition) {
          content.erase(wordStart, cursorPosition - wordStart);
          cursorPosition = wordStart;
          if (OnTextChange)
            OnTextChange(content);
        }
        ctrlBackspaceHeld = true;
        ctrlBackspaceTimer = 0.0f;
      } else {
        ctrlBackspaceTimer += dt;
        if (ctrlBackspaceTimer >= keyRepeatDelay) {
          float repeatTime = ctrlBackspaceTimer - keyRepeatDelay;
          if (repeatTime >= keyRepeatRate) {
            int wordStart = findPreviousWordBoundary(cursorPosition);
            if (wordStart < cursorPosition) {
              content.erase(wordStart, cursorPosition - wordStart);
              cursorPosition = wordStart;
              if (OnTextChange)
                OnTextChange(content);
            }
            ctrlBackspaceTimer = keyRepeatDelay;
          }
        }
      }
    } else {
      ctrlBackspaceHeld = false;
      ctrlBackspaceTimer = 0.0f;
    }

    const bool backspaceDown = !ctrlDown && ::IsKeyDown(::KEY_BACKSPACE);
    if (backspaceDown) {
      if (!backspaceHeld) {
        if (!content.empty() && cursorPosition > 0) {
          content.erase(cursorPosition - 1, 1);
          cursorPosition--;
          if (OnTextChange)
            OnTextChange(content);
        }
        backspaceHeld = true;
        backspaceTimer = 0.0f;
      } else {
        backspaceTimer += dt;
        if (backspaceTimer >= keyRepeatDelay) {
          float repeatTime = backspaceTimer - keyRepeatDelay;
          if (repeatTime >= keyRepeatRate) {
            if (!content.empty() && cursorPosition > 0) {
              content.erase(cursorPosition - 1, 1);
              cursorPosition--;
              if (OnTextChange)
                OnTextChange(content);
            }
            backspaceTimer = keyRepeatDelay;
          }
        }
      }
    } else {
      backspaceHeld = false;
      backspaceTimer = 0.0f;
    }

    const bool ctrlDeleteDown = ctrlDown && ::IsKeyDown(::KEY_DELETE);
    if (ctrlDeleteDown) {
      if (!ctrlDeleteHeld) {
        int wordEnd = findNextWordBoundary(cursorPosition);
        if (wordEnd > cursorPosition) {
          content.erase(cursorPosition, wordEnd - cursorPosition);
          if (OnTextChange)
            OnTextChange(content);
        }
        ctrlDeleteHeld = true;
        ctrlDeleteTimer = 0.0f;
      } else {
        ctrlDeleteTimer += dt;
        if (ctrlDeleteTimer >= keyRepeatDelay) {
          float repeatTime = ctrlDeleteTimer - keyRepeatDelay;
          if (repeatTime >= keyRepeatRate) {
            int wordEnd = findNextWordBoundary(cursorPosition);
            if (wordEnd > cursorPosition) {
              content.erase(cursorPosition, wordEnd - cursorPosition);
              if (OnTextChange)
                OnTextChange(content);
            }
            ctrlDeleteTimer = keyRepeatDelay;
          }
        }
      }
    } else {
      ctrlDeleteHeld = false;
      ctrlDeleteTimer = 0.0f;
    }

    const bool deleteDown = !ctrlDown && ::IsKeyDown(::KEY_DELETE);
    if (deleteDown) {
      if (!deleteHeld) {
        if (cursorPosition < static_cast<int>(content.length())) {
          content.erase(cursorPosition, 1);
          if (OnTextChange)
            OnTextChange(content);
        }
        deleteHeld = true;
        deleteTimer = 0.0f;
      } else {
        deleteTimer += dt;
        if (deleteTimer >= keyRepeatDelay) {
          float repeatTime = deleteTimer - keyRepeatDelay;
          if (repeatTime >= keyRepeatRate) {
            if (cursorPosition < static_cast<int>(content.length())) {
              content.erase(cursorPosition, 1);
              if (OnTextChange)
                OnTextChange(content);
            }
            deleteTimer = keyRepeatDelay;
          }
        }
      }
    } else {
      deleteHeld = false;
      deleteTimer = 0.0f;
    }

    if (::IsKeyPressed(::KEY_ENTER) && !content.empty()) {
      if (OnSubmit) {
        OnSubmit();
      }
    }

    const int key = ::GetCharPressed();
    if (key > 0) {
      content.insert(cursorPosition, 1, static_cast<char>(key));
      cursorPosition++;
      if (OnTextChange)
        OnTextChange(content);
    }

    if (isNavigating) {
      blinkVisible = true;
      blinkTimer = 0.0f;
    } else {
      blinkTimer += dt;
      if (blinkTimer >= blinkInterval) {
        blinkVisible = !blinkVisible;
        blinkTimer -= blinkInterval;
      }
    }

    if (textElement && cursorElement) {
      std::string textToCursor = content.substr(0, cursorPosition);
      float cursorPixelPos = MeasureTextEx(::GetFontDefault(), textToCursor.c_str(),
                                          static_cast<float>(textElement->defaultStyle->fontSize), 1.0f).x;

      const int containerPadding = GetStyle().padding;
      auto availableWidth = static_cast<float>(defaultStyle->width - (containerPadding * 2));
      const float scrollMargin = 20.0f;

      if (cursorPixelPos - textOffsetX > availableWidth - scrollMargin) {
        textOffsetX = cursorPixelPos - availableWidth + scrollMargin;
      } else if (cursorPixelPos - textOffsetX < scrollMargin) {
        textOffsetX = std::max(0.0f, cursorPixelPos - scrollMargin);
      }

      textElement->InnerText = content;

      const int containerHeight = GetStyle().height;
      const int textHeight = textElement->defaultStyle->fontSize;
      const int verticalCenter = (containerHeight - textHeight) / 2;

      if (textElement->inlineStyle) {
        textElement->inlineStyle->position.x = containerPadding - static_cast<int>(textOffsetX);
        textElement->inlineStyle->position.y = verticalCenter;
      }

      if (cursorElement->inlineStyle) {
        float exactCursorPos = cursorPixelPos - textOffsetX;
        cursorElement->inlineStyle->position.x = containerPadding + static_cast<int>(exactCursorPos);
        cursorElement->inlineStyle->position.y = verticalCenter;
      }

      if (cursorElement->defaultStyle) {
        cursorElement->defaultStyle->visible = blinkVisible;
      }
    }
  }

  int TextInput::findNextWordBoundary(int position) {
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

  int TextInput::findPreviousWordBoundary(int position) {
    if (position <= 0) {
      return 0;
    }

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