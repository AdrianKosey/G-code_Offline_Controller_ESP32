#include "toggle_widget.h"

void drawToggleVisual(DisplayManager& display, const Rect& bounds, bool state)
{
    uint16_t trackColor = state ? Theme::Progress : Theme::SidebarBackground;
    int16_t radius = bounds.height / 2;

    display.fillRoundRect(bounds.x, bounds.y, bounds.width, bounds.height, radius, trackColor);

    int16_t knobDiameter = bounds.height - 4;
    int16_t knobX = state ? (bounds.x + bounds.width - knobDiameter - 2) : (bounds.x + 2);
    int16_t knobY = bounds.y + 2;

    display.fillCircle(knobX + knobDiameter / 2, knobY + knobDiameter / 2, knobDiameter / 2, Theme::Text);
}

ToggleWidget::ToggleWidget(const Rect& bounds, bool initialState)
    : Widget(bounds), state(initialState)
{}

void ToggleWidget::setState(bool value)
{
    if (state == value)
        return;

    state = value;
    invalidate();
}

bool ToggleWidget::getState() const { return state; }

void ToggleWidget::setOnChange(ChangeCallback callback) { onChange = callback; }

bool ToggleWidget::handleTouch(const TouchEvent& event)
{
    if (!enabled)
        return false;

    if (event.type != TouchType::Pressed)
        return false;

    if (!bounds.contains(event.point.x, event.point.y))
        return false;

    state = !state;
    invalidate();

    if (onChange)
        onChange(state);

    return true;
}

void ToggleWidget::draw(DisplayManager& display)
{
    if (!dirty)
        return;

    bool visualState = enabled && state;
    uint16_t trackColor = enabled
        ? (visualState ? Theme::Progress : Theme::SidebarBackground)
        : Theme::Border;

    int16_t radius = bounds.height / 2;
    display.fillRoundRect(bounds.x, bounds.y, bounds.width, bounds.height, radius, trackColor);

    int16_t knobDiameter = bounds.height - 4;
    int16_t knobX = visualState ? (bounds.x + bounds.width - knobDiameter - 2) : (bounds.x + 2);
    int16_t knobY = bounds.y + 2;

    uint16_t knobColor = enabled ? Theme::Text : Theme::TextSecondary;
    display.fillCircle(knobX + knobDiameter / 2, knobY + knobDiameter / 2, knobDiameter / 2, knobColor);

    dirty = false;
}

void ToggleWidget::setEnabled(bool value)
{
    if (enabled == value)
        return;

    enabled = value;
    invalidate();
}

bool ToggleWidget::isEnabled() const { return enabled; }

