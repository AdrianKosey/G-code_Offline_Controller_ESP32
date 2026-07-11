#pragma once

#include "../../core/widget.h"
#include "../../theme.h"

class PanelWidget : public Widget
{
public:
    PanelWidget(
        const Rect& bounds,
        uint16_t backgroundColor = Theme::Panel,
        uint8_t radius = 8);

    void draw(DisplayManager& display) override;

private:
    uint16_t backgroundColor;
    uint8_t radius;
};