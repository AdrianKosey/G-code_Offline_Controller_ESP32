#pragma once

#include <Arduino.h>
#include "../../core/widget.h"
#include "../../theme.h"

class LabelWidget : public Widget
{
public:
    LabelWidget(
        const Rect& bounds,
        const String& text,
        uint16_t color = Theme::Text,
        uint8_t font = 2);

    void draw(DisplayManager& display) override;

    void setText(const String& value);
    void setColor(uint16_t value);

    void setBackground(uint16_t backgroundColor, uint8_t radius = 0);

private:
    String text;
    uint16_t color;
    uint8_t font;

    bool hasBackground = false;
    uint16_t backgroundColor = Theme::Background;
    uint8_t backgroundRadius = 0;
};