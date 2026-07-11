#include "label_widget.h"

LabelWidget::LabelWidget(const Rect& bounds, const String& text, uint16_t color, uint8_t font)
    : Widget(bounds), text(text), color(color), font(font)
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

void LabelWidget::setBackground(uint16_t bgColor, uint8_t radius)
{
    hasBackground = true;
    backgroundColor = bgColor;
    backgroundRadius = radius;

    invalidate();
}

void LabelWidget::draw(DisplayManager& display)
{
    if (!dirty)
        return;

    if (hasBackground)
    {
        display.fillRoundRect(
            bounds.x,
            bounds.y,
            bounds.width,
            bounds.height,
            backgroundRadius,
            backgroundColor);
    }

    // Approximate vertical centering. TFT_eSPI measures the actual text width
    // with tft.textWidth(), but that's inside the driver.
    int16_t textY = bounds.y + (bounds.height / 2) - (4 * font);

    display.drawText(text, bounds.x + 6, textY, color, font);

    dirty = false;
}