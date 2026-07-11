#include "label_widget.h"

LabelWidget::LabelWidget(const Rect& bounds, const String& text, uint16_t color, uint8_t font, uint16_t backgroundColor, bool allowTruncate)
    : Widget(bounds), text(text), color(color), font(font), backgroundColor(backgroundColor), allowTruncate(allowTruncate)
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

void LabelWidget::setAllowTruncate(bool allow)
{
    if (allowTruncate == allow)
        return;

    allowTruncate = allow;
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

    String displayText = text;

    if (allowTruncate)
    {
        int16_t maxWidth = bounds.width - 12;

        if (display.getTextWidth(displayText, font) > maxWidth)
        {
            while (displayText.length() > 0 &&
                   display.getTextWidth(displayText + "...", font) > maxWidth)
            {
                displayText.remove(displayText.length() - 1);
            }

            displayText += "...";
        }
    }

    display.drawText(displayText, bounds.x + 6, textY, color, font);

    dirty = false;
}