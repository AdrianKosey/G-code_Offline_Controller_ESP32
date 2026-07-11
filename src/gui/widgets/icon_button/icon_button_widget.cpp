#include "icon_button_widget.h"

IconButtonWidget::IconButtonWidget(
    const Rect& bounds,
    const uint8_t* icon,
    uint8_t iconWidth,
    uint8_t iconHeight,
    uint16_t backgroundColor,
    uint16_t iconColor,
    bool showBackground)
    : Widget(bounds),
      icon(icon), iconWidth(iconWidth), iconHeight(iconHeight),
      backgroundColor(backgroundColor), iconColor(iconColor), showBackground(showBackground)
{}

void IconButtonWidget::setOnPress(PressCallback callback)
{
    onPress = callback;
}

void IconButtonWidget::setBackgroundColor(uint16_t color)
{
    if (backgroundColor == color)
        return;

    backgroundColor = color;
    invalidate();
}

void IconButtonWidget::setIcon(const uint8_t* newIcon)
{
    if (icon == newIcon)
        return;

    icon = newIcon;
    invalidate();
}

bool IconButtonWidget::handleTouch(const TouchEvent& event)
{
    if (event.type != TouchType::Pressed)
        return false;

    if (!bounds.contains(event.point.x, event.point.y))
        return false;

    if (onPress)
        onPress();

    return true;
}

void IconButtonWidget::draw(DisplayManager& display)
{
    if (!dirty)
        return;

    int16_t cx = bounds.x + bounds.width / 2;
    int16_t cy = bounds.y + bounds.height / 2;

    if (showBackground)
    {
        int16_t radius = min(bounds.width, bounds.height) / 2;
        display.fillCircle(cx, cy, radius, backgroundColor);
    }
    else
    {
        display.fillRect(bounds.x, bounds.y, bounds.width, bounds.height, backgroundColor);
    }

    display.drawBitmap(
        cx - iconWidth / 2,
        cy - iconHeight / 2,
        icon,
        iconWidth,
        iconHeight,
        iconColor);

    dirty = false;
}