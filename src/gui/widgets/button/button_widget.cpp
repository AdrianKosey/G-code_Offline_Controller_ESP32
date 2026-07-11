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


void ButtonWidget::draw(DisplayManager& display)
{
    if(!dirty)
        return;

    uint16_t color =
        selected ?
        Theme::SidebarBorderSelected :
        Theme::SidebarBackground;

    display.fillRect(bounds.x, bounds.y, bounds.width, bounds.height, color);
    display.drawRect(bounds.x, bounds.y, bounds.width, bounds.height, Theme::Border);

    constexpr uint8_t font = 2;
    int16_t textY = bounds.y + (bounds.height / 2) - (4 * font); 

    display.drawText(text, bounds.x + 6, textY, Theme::Text, font);

    dirty = false;
}

bool ButtonWidget::handleTouch(const TouchEvent& event)
{
    if (event.type != TouchType::Pressed)
        return false;

    if (!contains(event.point.x, event.point.y))
        return false;

    if (onPress)
        onPress();

    return true;
}

void ButtonWidget::setOnPress(PressCallback callback)
{
    onPress = callback;
}