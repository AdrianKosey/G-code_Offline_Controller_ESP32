#include "label_widget.h"

LabelWidget::LabelWidget(const Rect& bounds, const String& text, uint16_t color, uint8_t font, uint16_t backgroundColor)
    : Widget(bounds), text(text), color(color), font(font), backgroundColor(backgroundColor)
{}

void LabelWidget::setText(const String& value)
{
    if (text == value)
        return;

    text = value;
    invalidate();
}

void LabelWidget::setColor(uint16_t value)
{
    if (color == value)
        return;

    color = value;
    invalidate();
}

void LabelWidget::setBackgroundColor(uint16_t value)
{
    if (backgroundColor == value)
        return;

    backgroundColor = value;
    invalidate();
}

void LabelWidget::setBadge(uint16_t color, uint8_t radius)
{
    hasBadge = true;
    badgeColor = color;
    badgeRadius = radius;

    invalidate();
}

void LabelWidget::draw(DisplayManager& display)
{
    if (!dirty)
        return;

    if (hasBadge)
    {
        display.fillRoundRect(bounds.x, bounds.y, bounds.width, bounds.height, badgeRadius, badgeColor);
    }
    else
    {
        display.fillRect(bounds.x, bounds.y, bounds.width, bounds.height, backgroundColor);
    }

    int16_t textY = bounds.y + (bounds.height / 2) - (4 * font);
    display.drawText(text, bounds.x + 6, textY, color, font);

    dirty = false;
}