#include "./ui/ExpandableTextInput.h"
#include "ui/UIFactory.h"
#include <limits>

namespace slam::ui {
  void ResizeHandler::Update(bool isHovered, Vector2 mousePosition, Vector2 elementPosition, Vector2 elementSize) {
    bool isMouseDown = ::IsMouseButtonDown(::MOUSE_BUTTON_LEFT);
    bool inResizeZone = IsInResizeZone(mousePosition, elementPosition, elementSize);

    if (inResizeZone && isMouseDown && !wasMouseDown) {
      isResizing = true;
      resizeStartPos = mousePosition;
      initialSize = elementSize;
    } else if (!isMouseDown && wasMouseDown) {
      isResizing = false;
    }

    if (isResizing) {
      Vector2 delta = {
        mousePosition.x - resizeStartPos.x,
        mousePosition.y - resizeStartPos.y
      };

      newSize = {
        initialSize.x + delta.x,
        initialSize.y + delta.y
      };
    } else {
      newSize = elementSize;
    }

    wasMouseDown = isMouseDown;
  }

  bool ResizeHandler::IsInResizeZone(Vector2 mousePosition, Vector2 elementPosition, Vector2 elementSize) const {
    Vector2 bottomRight = {
      elementPosition.x + elementSize.x,
      elementPosition.y + elementSize.y
    };

    return (mousePosition.x >= bottomRight.x - RESIZE_ZONE_SIZE &&
            mousePosition.x <= bottomRight.x + RESIZE_ZONE_SIZE &&
            mousePosition.y >= bottomRight.y - RESIZE_ZONE_SIZE &&
            mousePosition.y <= bottomRight.y + RESIZE_ZONE_SIZE);
  }

  void ResizeHandler::Reset() {
    isResizing = false;
    wasMouseDown = false;
    newSize = {0, 0};
  }

  void TextWrapper::UpdateLines(const std::string& content, float availableWidth, int fontSize) {
    lines.clear();
    WrapText(content, availableWidth, fontSize);
    if (lines.empty()) {
      lines.push_back({"", 0, 0});
    }
  }

  void TextWrapper::WrapText(const std::string& content, float availableWidth, int fontSize) {
    if (content.empty()) return;

    int currentIndex = 0;

    while (currentIndex <= static_cast<int>(content.length())) {
      int lineStart = currentIndex;
      int lineEnd = currentIndex;

      if (currentIndex < static_cast<int>(content.length()) && content[currentIndex] == '\n') {
        lines.push_back({"", lineStart, currentIndex});
        currentIndex++;
        continue;
      }

      while (lineEnd < static_cast<int>(content.length()) && content[lineEnd] != '\n') {
        lineEnd++;
      }

      if (lineStart == static_cast<int>(content.length())) {
        break;
      }

      std::string potentialLine = content.substr(lineStart, lineEnd - lineStart);

      if (potentialLine.empty()) {
        lines.push_back({"", lineStart, lineStart});
        currentIndex = lineEnd + (lineEnd < static_cast<int>(content.length()) ? 1 : 0);
        continue;
      }

      float lineWidth = MeasureTextEx(::GetFontDefault(), potentialLine.c_str(), static_cast<float>(fontSize), 1.0f).x;

      if (lineWidth <= availableWidth) {
        lines.push_back({potentialLine, lineStart, lineEnd});
        currentIndex = lineEnd + (lineEnd < static_cast<int>(content.length()) ? 1 : 0);
      } else {
        int wrapEnd = lineStart;
        int lastSpace = -1;

        while (wrapEnd < lineEnd) {
          if (content[wrapEnd] == ' ') {
            lastSpace = wrapEnd;
          }

          std::string testText = content.substr(lineStart, wrapEnd - lineStart + 1);
          float testWidth = MeasureTextEx(::GetFontDefault(), testText.c_str(), static_cast<float>(fontSize), 1.0f).x;

          if (testWidth > availableWidth) {
            if (lastSpace != -1 && lastSpace > lineStart) {
              std::string lineText = content.substr(lineStart, lastSpace - lineStart);
              lines.push_back({lineText, lineStart, lastSpace});
              currentIndex = lastSpace + 1;
            } else {
              if (wrapEnd == lineStart) wrapEnd++;
              std::string lineText = content.substr(lineStart, wrapEnd - lineStart);
              lines.push_back({lineText, lineStart, wrapEnd});
              currentIndex = wrapEnd;
            }
            break;
          }
          wrapEnd++;
        }

        if (wrapEnd == lineEnd) {
          std::string lineText = content.substr(lineStart, lineEnd - lineStart);
          lines.push_back({lineText, lineStart, lineEnd});
          currentIndex = lineEnd + (lineEnd < static_cast<int>(content.length()) ? 1 : 0);
        }
      }
    }
  }

  int TextWrapper::GetCursorLine(int cursorPosition) const {
    for (int i = 0; i < static_cast<int>(lines.size()); i++) {
      if (cursorPosition >= lines[i].startIndex && cursorPosition <= lines[i].endIndex) {
        return i;
      }
    }
    return std::max(0, static_cast<int>(lines.size()) - 1);
  }

  Vector2 TextWrapper::GetCursorPositionOnLine(int cursorPosition, int fontSize) const {
    int lineIndex = GetCursorLine(cursorPosition);
    if (lineIndex >= static_cast<int>(lines.size())) {
      return {0.0f, static_cast<float>(lineIndex * fontSize)};
    }

    const auto& line = lines[lineIndex];
    int positionInLine = cursorPosition - line.startIndex;

    if (positionInLine > static_cast<int>(line.text.length())) {
      positionInLine = static_cast<int>(line.text.length());
    }

    std::string textBeforeCursor = line.text.substr(0, positionInLine);

    float x = MeasureTextEx(::GetFontDefault(), textBeforeCursor.c_str(), static_cast<float>(fontSize), 1.0f).x;
    auto y = static_cast<float>(lineIndex * fontSize);

    return {x, y};
  }

  void ExpandableTextInput::OnCreate() {
    SetupContainer();
    SetupTextElement();
    SetupCursorElement();
    if (textElement) {
      lineHeight = textElement->defaultStyle->fontSize;
    }
    UpdateHeight();
  }

  void ExpandableTextInput::SetupContainer() {
    Factory::SetupInputContainer(this, 300, minHeight, 0.0f);
    defaultStyle->flex = false;
    defaultStyle->overflowHidden = true;
  }

  void ExpandableTextInput::SetupTextElement() {
    textElement = Factory::AddText(this, "", 16, ::BLACK);
  }

  void ExpandableTextInput::SetupCursorElement() {
    cursorElement = Factory::AddCursor(this, 16, ::BLACK);
  }

  void ExpandableTextInput::OnUpdate(float dt) {
    if (isResizableEnabled) {
      HandleResizing();
    }

    Vector2 mousePos = ::GetMousePosition();
    Vector2 elementPos = {static_cast<float>(AbsPosition.x), static_cast<float>(AbsPosition.y)};
    Vector2 elementSize = {static_cast<float>(GetStyle().width), static_cast<float>(GetStyle().height)};

    if (resizeHandler.IsResizing()) {
      SetMouseCursor(::MOUSE_CURSOR_RESIZE_NWSE);
    } else if (resizeHandler.IsInResizeZone(mousePos, elementPos, elementSize)) {
      SetMouseCursor(::MOUSE_CURSOR_RESIZE_NWSE);
    } else {
      SetMouseCursor(IsHovered ? ::MOUSE_CURSOR_IBEAM : ::MOUSE_CURSOR_DEFAULT);
    }

    if (!resizeHandler.IsResizing() && IsFocused) {
      HandleTextInput();
      HandleNavigation(dt);
      HandleEditing(dt);
    } else if (!IsFocused) {
      blinker.Reset();
      return;
    }

    UpdateTextLayout();
    UpdateHeight();
    UpdateScrolling();
  }

  void ExpandableTextInput::HandleResizing() {
    Vector2 mousePos = ::GetMousePosition();
    Vector2 elementPos = {static_cast<float>(AbsPosition.x), static_cast<float>(AbsPosition.y)};
    Vector2 elementSize = {static_cast<float>(GetStyle().width), static_cast<float>(GetStyle().height)};

    resizeHandler.Update(IsHovered, mousePos, elementPos, elementSize);

    if (resizeHandler.IsResizing()) {
      Vector2 newSize = resizeHandler.GetNewSize();

      int newWidth = std::max(minWidth_, std::min(maxWidth_, static_cast<int>(newSize.x)));
      int newHeight = std::max(minHeight_, std::min(maxHeight_, static_cast<int>(newSize.y)));

      if (inlineStyle) {
        inlineStyle->width = newWidth;
        inlineStyle->height = newHeight;
      } else {
        defaultStyle->width = newWidth;
        defaultStyle->height = newHeight;
      }

      currentHeight = newHeight;
    }
  }

  void ExpandableTextInput::HandleTextInput() {
    const int key = ::GetCharPressed();
    if (key > 0) {
      if (key == '\r' || key == '\n') {
        InsertNewline();
      } else {
        InsertText(static_cast<char>(key));
      }
    }

    if (::IsKeyPressed(::KEY_ENTER)) {
      InsertNewline();
    }
  }

  void ExpandableTextInput::HandleNavigation(float dt) {
    const bool ctrlDown = ::IsKeyDown(::KEY_LEFT_CONTROL) || ::IsKeyDown(::KEY_RIGHT_CONTROL);
    bool isNavigating = false;

    if (::IsKeyPressed(::KEY_HOME)) {
      if (ctrlDown) {
        MoveCursor(0);
      } else {
        MoveCursorToLineStart();
      }
      isNavigating = true;
    }
    if (::IsKeyPressed(::KEY_END)) {
      if (ctrlDown) {
        MoveCursor(static_cast<int>(content.length()));
      } else {
        MoveCursorToLineEnd();
      }
      isNavigating = true;
    }

    keyHandlers["up"].Update(dt, ::IsKeyDown(::KEY_UP), [&]() {
      MoveCursorUp();
      isNavigating = true;
    });

    keyHandlers["down"].Update(dt, ::IsKeyDown(::KEY_DOWN), [&]() {
      MoveCursorDown();
      isNavigating = true;
    });

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

  void ExpandableTextInput::HandleEditing(float dt) {
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

  void ExpandableTextInput::UpdateTextLayout() {
    if (!textElement || !cursorElement) return;

    const int containerPadding = GetStyle().padding;
    auto availableWidth = GetStyle().width - static_cast<float>(containerPadding * 2);

    wrapper.UpdateLines(content, availableWidth, textElement->defaultStyle->fontSize);

    std::string displayText = "";
    const auto& lines = wrapper.GetLines();
    for (size_t i = 0; i < lines.size(); i++) {
      displayText += lines[i].text;
      if (i < lines.size() - 1) {
        displayText += "\n";
      }
    }

    textElement->InnerText = displayText;

    if (textElement->inlineStyle) {
      const int fontSize = textElement->defaultStyle->fontSize;
      int extraOffset = (lineHeight - fontSize) / 2;
      textElement->inlineStyle->position.x = containerPadding;
      textElement->inlineStyle->position.y = containerPadding + extraOffset - static_cast<int>(scrollOffsetY);
    }

    int fontSize = textElement->defaultStyle->fontSize;
    Vector2 textSize = MeasureTextEx(::GetFontDefault(), displayText.c_str(), static_cast<float>(fontSize), 1.0f);
    float actualLineHeight = textSize.y / static_cast<float>(wrapper.GetLineCount());
    float cursorX = wrapper.GetCursorPositionOnLine(cursorPosition, fontSize).x;
    int lineIndex = wrapper.GetCursorLine(cursorPosition);
    float cursorY = lineIndex * actualLineHeight;
    if (cursorElement->inlineStyle) {
      cursorElement->inlineStyle->position.x = containerPadding + static_cast<int>(cursorX);
      cursorElement->inlineStyle->position.y = containerPadding + static_cast<int>(cursorY) - static_cast<int>(scrollOffsetY);
    }

    if (cursorElement->defaultStyle) {
      cursorElement->defaultStyle->visible = blinker.IsVisible();
    }
  }

  void ExpandableTextInput::UpdateHeight() {
    int lineCount = std::max(1, wrapper.GetLineCount());
    int requiredHeight = (lineCount * lineHeight) + (GetStyle().padding * 2);

    int newHeight = std::max(minHeight, std::min(maxHeight, requiredHeight));

    if (newHeight != currentHeight) {
      currentHeight = newHeight;
      this->defaultStyle->height = newHeight;
    }
  }

  void ExpandableTextInput::UpdateScrolling() {
    const int fontSize = textElement->defaultStyle->fontSize;
    const int containerPadding = GetStyle().padding;
    const int visibleHeight = currentHeight - (containerPadding * 2);
    std::string displayText = textElement->InnerText;
    Vector2 textSizeScroll = MeasureTextEx(::GetFontDefault(), displayText.c_str(), static_cast<float>(fontSize), 1.0f);
    float actualLineHeightScroll = textSizeScroll.y / static_cast<float>(wrapper.GetLineCount());
    int scrollLine = wrapper.GetCursorLine(cursorPosition);
    float cursorYScroll = scrollLine * actualLineHeightScroll;
    if (cursorYScroll - scrollOffsetY > visibleHeight) {
      scrollOffsetY = cursorYScroll - visibleHeight;
    } else if (cursorYScroll - scrollOffsetY < 0) {
      scrollOffsetY = cursorYScroll;
    }

    float maxScroll = std::max(0.0f, (static_cast<float>(wrapper.GetLineCount() * lineHeight) - visibleHeight));
    scrollOffsetY = std::max(0.0f, std::min(maxScroll, scrollOffsetY));
  }

  void ExpandableTextInput::MoveCursor(int newPosition) {
    cursorPosition = std::max(0, std::min(static_cast<int>(content.length()), newPosition));
  }

  void ExpandableTextInput::MoveCursorUp() {
    Vector2 currentPos = wrapper.GetCursorPositionOnLine(cursorPosition, textElement->defaultStyle->fontSize);
    int currentLine = wrapper.GetCursorLine(cursorPosition);

    if (currentLine > 0) {
      const auto& lines = wrapper.GetLines();
      const auto& targetLine = lines[currentLine - 1];

      float bestDistance = std::numeric_limits<float>::max();
      int bestPosition = targetLine.startIndex;

      for (int pos = targetLine.startIndex; pos <= targetLine.endIndex; pos++) {
        Vector2 testPos = wrapper.GetCursorPositionOnLine(pos, textElement->defaultStyle->fontSize);
        float distance = std::abs(testPos.x - currentPos.x);
        if (distance < bestDistance) {
          bestDistance = distance;
          bestPosition = pos;
        }
      }

      MoveCursor(bestPosition);
    }
  }

  void ExpandableTextInput::MoveCursorDown() {
    Vector2 currentPos = wrapper.GetCursorPositionOnLine(cursorPosition, textElement->defaultStyle->fontSize);
    int currentLine = wrapper.GetCursorLine(cursorPosition);

    if (currentLine < wrapper.GetLineCount() - 1) {
      const auto& lines = wrapper.GetLines();
      const auto& targetLine = lines[currentLine + 1];

      float bestDistance = std::numeric_limits<float>::max();
      int bestPosition = targetLine.startIndex;

      for (int pos = targetLine.startIndex; pos <= targetLine.endIndex; pos++) {
        Vector2 testPos = wrapper.GetCursorPositionOnLine(pos, textElement->defaultStyle->fontSize);
        float distance = std::abs(testPos.x - currentPos.x);
        if (distance < bestDistance) {
          bestDistance = distance;
          bestPosition = pos;
        }
      }

      MoveCursor(bestPosition);
    }
  }

  void ExpandableTextInput::MoveCursorToLineStart() {
    int currentLine = wrapper.GetCursorLine(cursorPosition);
    if (currentLine < wrapper.GetLineCount()) {
      const auto& lines = wrapper.GetLines();
      MoveCursor(lines[currentLine].startIndex);
    }
  }

  void ExpandableTextInput::MoveCursorToLineEnd() {
    int currentLine = wrapper.GetCursorLine(cursorPosition);
    if (currentLine < wrapper.GetLineCount()) {
      const auto& lines = wrapper.GetLines();
      MoveCursor(lines[currentLine].endIndex);
    }
  }

  void ExpandableTextInput::InsertText(char character) {
    content.insert(cursorPosition, 1, character);
    cursorPosition++;
    if (OnTextChange) OnTextChange(content);
  }

  void ExpandableTextInput::InsertNewline() {
    content.insert(cursorPosition, 1, '\n');
    cursorPosition++;
    if (OnTextChange) OnTextChange(content);
  }

  void ExpandableTextInput::DeleteText(int startPos, int length) {
    if (startPos >= 0 && startPos < static_cast<int>(content.length())) {
      content.erase(startPos, length);
      if (OnTextChange) OnTextChange(content);
    }
  }

  void ExpandableTextInput::DeletePreviousWord() {
    int wordStart = FindPreviousWordBoundary(cursorPosition);
    if (wordStart < cursorPosition) {
      DeleteText(wordStart, cursorPosition - wordStart);
      cursorPosition = wordStart;
    }
  }

  void ExpandableTextInput::DeleteNextWord() {
    int wordEnd = FindNextWordBoundary(cursorPosition);
    if (wordEnd > cursorPosition) {
      DeleteText(cursorPosition, wordEnd - cursorPosition);
    }
  }

  int ExpandableTextInput::FindNextWordBoundary(int position) const {
    if (position >= static_cast<int>(content.length())) {
      return static_cast<int>(content.length());
    }

    while (position < static_cast<int>(content.length()) &&
           (std::isalnum(content[position]) || content[position] == '_')) {
      position++;
    }

    while (position < static_cast<int>(content.length()) &&
           !std::isalnum(content[position]) && content[position] != '_' && content[position] != '\n') {
      position++;
    }

    return position;
  }

  int ExpandableTextInput::FindPreviousWordBoundary(int position) const {
    if (position <= 0) return 0;

    position--;

    while (position > 0 &&
           !std::isalnum(content[position]) && content[position] != '_' && content[position] != '\n') {
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
