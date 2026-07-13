#include "menu_row_widget.h"

MenuRowWidget::MenuRowWidget(const Rect& bounds, const String& label)
    : Widget(bounds), label(label)
{}

void MenuRowWidget::setOnPress(PressCallback callback) { onPress = callback; }

bool MenuRowWidget::handleTouch(const TouchEvent& event)
{
    if (event.type == TouchType::Pressed)
    {
        if (!bounds.contains(event.point.x, event.point.y))
            return false;

        pressed = true;
        invalidate();
        return true;
    }

    if (event.type == TouchType::Released && pressed)
    {
        pressed = false;
        invalidate();

        if (onPress)
            onPress();

        return true;
    }

    return false;
}

void MenuRowWidget::draw(DisplayManager& display)
{
    if (!dirty) return;

    uint16_t bg = pressed ? Theme::SidebarSelected : Theme::Panel;
    display.fillRect(bounds.x, bounds.y, bounds.width, bounds.height, bg);

    display.drawText(label, bounds.x + 12, bounds.y + bounds.height / 2 - 8, Theme::Text, 2);

    if (value.length() > 0)
    {
        display.drawText(value, bounds.x + bounds.width - 90, bounds.y + bounds.height / 2 - 8, Theme::Progress, 2);
    }
    else
    {
        int16_t cx = bounds.x + bounds.width - 16;
        int16_t cy = bounds.y + bounds.height / 2;
        display.drawLine(cx, cy - 5, cx + 5, cy, Theme::TextSecondary);
        display.drawLine(cx + 5, cy, cx, cy + 5, Theme::TextSecondary);
    }

    display.drawLine(bounds.x, bounds.y + bounds.height, bounds.x + bounds.width, bounds.y + bounds.height, Theme::Border);

    dirty = false;
}

void MenuRowWidget::setValue(const String& newValue)
{
    if (value == newValue) return;
    value = newValue;
    invalidate();
}