#include "../../include/ui/Element.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


namespace slam::ui {
  void Panel::Draw(const Vector2 offset) {
    GuiPanel({position.x + offset.x, position.y + offset.y, static_cast<float>(width), static_cast<float>(height)}, "My Panel");
  }
}
