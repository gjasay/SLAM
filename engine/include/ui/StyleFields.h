#pragma once

#define STYLE_PROPERTIES                                                                                               \
  STYLE_FIELD(Vector2, position, {0, 0})                                                                               \
  STYLE_FIELD(float, width, 0.0f)                                                                                      \
  STYLE_FIELD(float, height, 0.0f)                                                                                     \
  STYLE_FIELD(int, zIndex, 0)                                                                                          \
  STYLE_FIELD(int, borderWidth, 0)                                                                                     \
  STYLE_FIELD(int, padding, 0)                                                                                         \
  STYLE_FIELD(int, margin, 0)                                                                                          \
  STYLE_FIELD(int, marginTop, 0)                                                                                       \
  STYLE_FIELD(int, marginBottom, 0)                                                                                    \
  STYLE_FIELD(int, marginLeft, 0)                                                                                      \
  STYLE_FIELD(int, marginRight, 0)                                                                                     \
  STYLE_FIELD(Color, backgroundColor, {0, 0, 0, 0})                                                                    \
  STYLE_FIELD(Color, borderColor, {0, 0, 0, 0})                                                                        \
  STYLE_FIELD(Color, color, {0, 0, 0, 0})                                                                              \
  STYLE_FIELD(int, fontSize, 20)                                                                                       \
  STYLE_FIELD(float, borderRadius, 0.0f)                                                                               \
  STYLE_FIELD(bool, flex, false)                                                                                       \
  STYLE_FIELD(int, flexGap, 0)                                                                                         \
  STYLE_FIELD(ui::FlexDirection, flexDirection, FlexDirection::Row)                                                    \
  STYLE_FIELD(ui::JustifyContent, justifyContent, JustifyContent::FlexStart)                                           \
  STYLE_FIELD(ui::AlignItems, alignItems, AlignItems::FlexStart)                                                       \
  STYLE_FIELD(bool, visible, true)                                                                                     \
  STYLE_FIELD(bool, widthIsPercent, false)                                                                             \
  STYLE_FIELD(float, widthPercent, 0.0f)                                                                               \
  STYLE_FIELD(bool, heightIsPercent, false)                                                                            \
  STYLE_FIELD(float, heightPercent, 0.0f)                                                                              \
  STYLE_FIELD(bool, overflowHidden, false)                                                                             \
  STYLE_FIELD(bool, absolutePosition, false)
