#pragma once

#include "../../core/widget.h"
#include "../../theme.h"

class ProgressBarWidget : public Widget
{
public:
    ProgressBarWidget(
        const Rect& bounds,
        uint16_t trackColor = Theme::SidebarBackground,
        uint16_t fillColor = Theme::Progress);

    void draw(DisplayManager& display) override;

    void setValue(float percent); // 0.0 - 100.0
    float getValue() const;

private:
    uint16_t trackColor;
    uint16_t fillColor;

    float value = 0.0f;
};