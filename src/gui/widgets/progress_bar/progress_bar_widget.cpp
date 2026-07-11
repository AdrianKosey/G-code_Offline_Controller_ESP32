#include "progress_bar_widget.h"
#include <algorithm>

ProgressBarWidget::ProgressBarWidget(const Rect& bounds, uint16_t trackColor, uint16_t fillColor)
    : Widget(bounds), trackColor(trackColor), fillColor(fillColor)
{}

void ProgressBarWidget::setValue(float percent)
{
    percent = constrain(percent, 0.0f, 100.0f);

    if (percent == value)
        return;

    value = percent;
    invalidate();
}

float ProgressBarWidget::getValue() const
{
    return value;
}

void ProgressBarWidget::draw(DisplayManager& display)
{
    if (!dirty)
        return;

    int16_t r = bounds.height / 2;

    display.fillRoundRect(bounds.x, bounds.y, bounds.width, bounds.height, r, trackColor);

    int16_t fillWidth = (int16_t)(bounds.width * (value / 100.0f));

    if (fillWidth > 0)
    {
        // radius limited to the current width so that it doesn't look distorted when the padding is very small
        int16_t fillRadius = std::min<int16_t>(r, fillWidth / 2);
        display.fillRoundRect(bounds.x, bounds.y, fillWidth, bounds.height, fillRadius, fillColor);
    }

    dirty = false;
}