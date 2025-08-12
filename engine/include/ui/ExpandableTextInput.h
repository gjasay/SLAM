#pragma once
#include "ui/TextInput.h"
#include <vector>

namespace slam::ui {
  class ResizeHandler {
  public:
    void Update(bool isHovered, Vector2 mousePosition, Vector2 elementPosition, Vector2 elementSize);
    bool IsResizing() const { return isResizing; }
    Vector2 GetNewSize() const { return newSize; }
    bool IsInResizeZone(Vector2 mousePosition, Vector2 elementPosition, Vector2 elementSize) const;
    void Reset();

  private:
    bool isResizing = false;
    bool wasMouseDown = false;
    Vector2 resizeStartPos = {0, 0};
    Vector2 initialSize = {0, 0};
    Vector2 newSize = {0, 0};
    static constexpr float RESIZE_ZONE_SIZE = 10.0f; // Size of resize handle area
  };

  class TextWrapper {
  public:
    struct LineInfo {
      std::string text;
      int startIndex;
      int endIndex;
    };

    void UpdateLines(const std::string& content, float availableWidth, int fontSize);
    const std::vector<LineInfo>& GetLines() const { return lines; }
    int GetLineCount() const { return static_cast<int>(lines.size()); }
    int GetCursorLine(int cursorPosition) const;
    Vector2 GetCursorPositionOnLine(int cursorPosition, int fontSize) const;

  private:
    std::vector<LineInfo> lines;
    void WrapText(const std::string& content, float availableWidth, int fontSize);
  };

  class ExpandableTextInput : public Panel {
  public:
    ExpandableTextInput() = default;
    void OnCreate() override;
    void OnUpdate(float dt) override;

    std::function<void(const std::string&)> OnTextChange;
    std::function<void()> OnSubmit;

    // Configuration
    void SetMinHeight(int height) { minHeight = height; }
    void SetMaxHeight(int height) { maxHeight = height; }
    void SetLineHeight(int height) { lineHeight = height; }

    // Resize configuration
    void SetResizable(bool resizable) { isResizableEnabled = resizable; }
    bool IsResizable() const { return isResizableEnabled; }
    void SetMinSize(int minWidth, int minHeight) {
      minWidth_ = minWidth;
      minHeight_ = minHeight;
    }
    void SetMaxSize(int maxWidth, int maxHeight) {
      maxWidth_ = maxWidth;
      maxHeight_ = maxHeight;
    }

  private:
    Text* textElement = nullptr;
    Text* cursorElement = nullptr;

    std::string content;
    int cursorPosition = 0;

    int minHeight = 40;
    int maxHeight = 300;
    int lineHeight = 25;
    int currentHeight = 40;

    // Resize functionality
    bool isResizableEnabled = true;
    ResizeHandler resizeHandler;
    int minWidth_ = 100;
    int maxWidth_ = 800;
    int minHeight_ = 40;
    int maxHeight_ = 600;

    TextWrapper wrapper;
    CursorBlinker blinker;
    std::unordered_map<std::string, KeyRepeatHandler> keyHandlers;

    float scrollOffsetY = 0.0f;

    void HandleTextInput();
    void HandleNavigation(float dt);
    void HandleEditing(float dt);
    void HandleResizing();
    void UpdateTextLayout();
    void UpdateScrolling();
    void UpdateHeight();

    int FindNextWordBoundary(int position) const;
    int FindPreviousWordBoundary(int position) const;
    void MoveCursor(int newPosition);
    void MoveCursorUp();
    void MoveCursorDown();
    void MoveCursorToLineStart();
    void MoveCursorToLineEnd();

    void InsertText(char character);
    void InsertNewline();
    void DeleteText(int startPos, int length);
    void DeletePreviousWord();
    void DeleteNextWord();

    void SetupTextElement();
    void SetupCursorElement();
    void SetupContainer();
  };
}
