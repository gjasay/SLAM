#include "ui/UIFactory.h"
#include "ui/BasicElements.h"
#include <utility>

namespace slam::ui::Factory {
    Text* AddText(Element* parent, const std::string& text, int fontSize, Color color, bool absolutePosition, int marginLeft) {
        auto textPtr = std::make_unique<Text>(text);
        Text* textElem = dynamic_cast<Text*>(parent->AddChild(std::move(textPtr)));
        if (textElem && textElem->defaultStyle) {
            auto& style = *textElem->defaultStyle;
            style.fontSize = fontSize;
            style.color = color;
            style.padding = 0;
            style.marginLeft = marginLeft;
            style.absolutePosition = absolutePosition;
        }
        return textElem;
    }

    Text* AddCursor(Element* parent, int fontSize, Color color) {
        auto cursorPtr = std::make_unique<Text>("|");
        Text* cursorElem = dynamic_cast<Text*>(parent->AddChild(std::move(cursorPtr)));
        if (cursorElem && cursorElem->defaultStyle) {
            auto& style = *cursorElem->defaultStyle;
            style.fontSize = fontSize;
            style.color = color;
            style.padding = 0;
            style.marginLeft = 0;
            style.absolutePosition = true;
            style.zIndex = 1;
        }
        return cursorElem;
    }

    void SetupInputContainer(Panel* panel, int width, int height, float borderRadius) {
        if (!panel->defaultStyle) return;
        auto& style = *panel->defaultStyle;
        style.borderColor = GRAY;
        style.borderWidth = 1;
        style.borderRadius = borderRadius;
        style.width = width;
        style.height = height;
        style.padding = 10;
        style.flex = false;
        style.overflowHidden = true;
    }
}
