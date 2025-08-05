#include "../../include/ui/Canvas.h"
#include "raylib.h"

namespace slam::ui {
  void Canvas::Draw() { root->_draw({0, 0}); }

  void Canvas::Update(float dt) {
    root->_update(dt);
  }

  Element *Canvas::AddElement(std::unique_ptr<Element> element) { return root->AddChild(std::move(element)); }

} // namespace slam::ui
