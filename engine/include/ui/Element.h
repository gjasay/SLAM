#pragma once
#include <algorithm>
#include <memory>
#include <vector>
#include "Style.h"
#include "Vector.h"

namespace slam::ui {
  class Canvas;

  class Element {
  public:
    Element() = default;
    virtual ~Element() = default;

    std::string id;
    std::vector<std::string> classes;

    Canvas *canvas = nullptr;

    std::unique_ptr<Style> defaultStyle = std::make_unique<Style>();
    std::unique_ptr<Style> inlineStyle = std::make_unique<Style>();
    std::unique_ptr<Style> hoverStyle = std::make_unique<Style>();
    std::unique_ptr<Style> activeStyle = std::make_unique<Style>();
    std::unique_ptr<Style> focusStyle = std::make_unique<Style>();
    std::unique_ptr<Style> disabledStyle = std::make_unique<Style>();

    bool IsHovered = false;
    bool IsActive = false;
    bool IsFocused = false;
    bool IsDisabled = false;
    Vector2 AbsPosition = {0, 0};

    [[nodiscard]] StyleState GetCurrentState() const {
      if (IsDisabled)
        return StyleState::Disabled;
      if (IsActive)
        return StyleState::Active;
      if (IsHovered)
        return StyleState::Hover;
      if (IsFocused)
        return StyleState::Focus;
      return StyleState::Default;
    }
    [[nodiscard]] Style GetStyle() const { return resolvedStyle; }

    virtual void OnCreate() {}
    virtual void OnUpdate(float dt) {}
    virtual void OnDestroy() {}

    void _update(float dt, Vector2 offset = {0, 0});

    Element *AddChild(std::unique_ptr<Element> child) {
      child->canvas = canvas;
      children.emplace_back(std::move(child));
      children.back()->OnCreate();
      return children.back().get();
    }

    void RemoveChild(Element *child) {
      children.erase(std::remove_if(children.begin(), children.end(),
                                    [child](const std::unique_ptr<Element> &c) { return c.get() == child; }),
                     children.end());
    }

    void RemoveChildren() { children.clear(); }

    void AddClass(const std::string &className) { classes.emplace_back(className); }

    void RemoveClass(const std::string &className) {
      classes.erase(std::remove(classes.begin(), classes.end(), className), classes.end());
    }

    virtual void Draw(Style style, Vector2 offset) {}

  protected:
    std::vector<std::unique_ptr<Element>> children;

  private:
    void _draw(Vector2 offset = {0, 0});
    void resolvePercentSizes(const Style &finalStyle) const;
    void applyFlexLayout(Style &finalStyle);
    void calculateFlexMeasurements(Style &finalStyle, bool isRow, int &totalChildSize, int &containerSize,
                                   int &remainingSpace);
    void calculateFlexOffsets(Style &finalStyle, int remainingSpace, float &mainAxisOffset);
    void layoutFlexChildren(Style &finalStyle, bool isRow, float mainAxisOffset);
    void renderElements(const Style &finalStyle, const Vector2 &offset);
    Style resolvedStyle = Style();

    friend class Canvas;
  };


} // namespace slam::ui
