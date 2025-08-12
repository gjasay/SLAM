#pragma once
#include <string>
#include "ui/BasicElements.h"
#include "raylib.h"

namespace slam::ui::Factory {
    void SetupInputContainer(Panel* panel, int width, int height, float borderRadius);
    Text* AddText(Element* parent, const std::string& text, int fontSize, Color color, bool absolutePosition = true, int marginLeft = 0);
    Text* AddCursor(Element* parent, int fontSize, Color color);
}
