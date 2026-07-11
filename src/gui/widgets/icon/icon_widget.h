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
        uint16_t color = Theme::Text);

    void draw(DisplayManager& display) override;

    void setColor(uint16_t color);

private:
    const uint8_t* icon;
    uint8_t iconWidth;
    uint8_t iconHeight;
    uint16_t color;
};