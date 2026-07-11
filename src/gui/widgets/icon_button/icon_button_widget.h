#pragma once

#include <Arduino.h>
#include <functional>

#include "../../core/widget.h"
#include "../../theme.h"

class IconButtonWidget : public Widget
{
public:
    using PressCallback = std::function<void()>;

    IconButtonWidget(
        const Rect& bounds,
        const uint8_t* icon,
        uint8_t iconWidth,
        uint8_t iconHeight,
        uint16_t backgroundColor,
        uint16_t iconColor = Theme::Text,
        bool showBackground = true);

    void draw(DisplayManager& display) override;
    bool handleTouch(const TouchEvent& event) override;

    void setOnPress(PressCallback callback);
    void setBackgroundColor(uint16_t color);
    void setIcon(const uint8_t* icon);

private:
    const uint8_t* icon;
    uint8_t iconWidth;
    uint8_t iconHeight;

    uint16_t backgroundColor; // If showBackground=false, it is only used to CLEAR the area (background color behind)
    uint16_t iconColor;
    bool showBackground;

    PressCallback onPress;
};