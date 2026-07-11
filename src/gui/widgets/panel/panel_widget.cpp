#include "panel_widget.h"

PanelWidget::PanelWidget(const Rect& bounds, uint16_t backgroundColor, uint8_t radius)
    : Widget(bounds), backgroundColor(backgroundColor), radius(radius)
{}

void PanelWidget::draw(DisplayManager& display)
{
    if (!dirty)
        return;

    display.fillRoundRect(
        bounds.x,
        bounds.y,
        bounds.width,
        bounds.height,
        radius,
        backgroundColor);

    dirty = false;
}