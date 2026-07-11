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
        uint8_t font = 2,
        uint16_t backgroundColor = Theme::Background,
        bool allowTruncate = true);

    void draw(DisplayManager& display) override;

    void setText(const String& value);
    void setColor(uint16_t value);
    void setBackgroundColor(uint16_t value);
    void setAllowTruncate(bool allow);

    void setBadge(uint16_t badgeColor, uint8_t radius);

private:
    String text;
    uint16_t color;
    uint8_t font;
    uint16_t backgroundColor;
    bool allowTruncate;

    bool hasBadge = false;
    uint16_t badgeColor = Theme::Background;
    uint8_t badgeRadius = 0;
};