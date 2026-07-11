#pragma once

#include "../../core/widget.h"
#include "../../theme.h"

class IconWidget : public Widget
{
public:
    IconWidget(
        const Rect& bounds,
        const uint8_t* icon,
        uint8_t iconWidth,
        uint8_t iconHeight,
        uint16_t color = Theme::Text,
        uint16_t backgroundColor = Theme::Background);

    void draw(DisplayManager& display) override;

    void setColor(uint16_t color);
    void setIcon(const uint8_t* icon);

private:
    const uint8_t* icon;
    uint8_t iconWidth;
    uint8_t iconHeight;
    uint16_t color;
    uint16_t backgroundColor;
};