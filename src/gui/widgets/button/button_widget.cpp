#include "button_widget.h"

ButtonWidget::ButtonWidget(const Rect& bounds, const String& text) : Widget(bounds), text(text)
{}

bool ButtonWidget::contains(
    uint16_t x,
    uint16_t y) const
{
    return bounds.contains(x,y);
}

void ButtonWidget::setSelected(bool value)
{
    if(selected == value)
        return;

    selected = value;

    invalidate();
}

bool ButtonWidget::isSelected() const
{
    return selected;
}

void ButtonWidget::setText(const String& value)
{
    if(text == value)
        return;

    text = value;

    invalidate();
}

bool ButtonWidget::handleTouch(const TouchEvent& event)
{
    if(event.type != TouchType::Pressed)
        return false;

    return contains(
        event.point.x,
        event.point.y);
}

void ButtonWidget::draw(DisplayManager& display)
{
    if(!dirty)
        return;

    uint16_t color =
        selected ?
        Theme::SidebarBorderSelected :
        Theme::SidebarBackground;

    display.fillRect(
        bounds.x,
        bounds.y,
        bounds.width,
        bounds.height,
        color);

    display.drawRect(
        bounds.x,
        bounds.y,
        bounds.width,
        bounds.height,
        Theme::Border);

    display.drawText(
        text,
        bounds.x + 10,
        bounds.y + 18,
        Theme::Text);

    dirty = false;
}