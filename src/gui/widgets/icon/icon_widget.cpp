#include "icon_widget.h"

IconWidget::IconWidget(const Rect& bounds, const uint8_t* icon, uint8_t iconWidth, uint8_t iconHeight, uint16_t color)
    : Widget(bounds), icon(icon), iconWidth(iconWidth), iconHeight(iconHeight), color(color)
{}

void IconWidget::setColor(uint16_t value)
{
    if (color == value)
        return;

    color = value;
    invalidate();
}

void IconWidget::draw(DisplayManager& display)
{
    if (!dirty)
        return;

    display.drawBitmap(bounds.x, bounds.y, icon, iconWidth, iconHeight, color);

    dirty = false;
}