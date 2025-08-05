#pragma once
#include <string>
#include <unordered_map>
#include "StyleFields.h"
#include "Vector.h"

namespace slam::ui {
  enum class StyleState { Default, Hover, Active, Focus, Disabled };
  class Element;

  enum class FlexDirection { Row, Column };
  enum class JustifyContent {
    FlexStart,
    FlexEnd,
    Center,
    SpaceBetween,
    SpaceAround,
    SpaceEvenly,
  };
  enum class AlignItems {
    FlexStart,
    FlexEnd,
    Center,
    Stretch,
    Baseline,
  };

  struct Style {
#define STYLE_FIELD(type, name, ...) type name = __VA_ARGS__;
    STYLE_PROPERTIES
#undef STYLE_FIELD
  };


} // namespace slam::ui
