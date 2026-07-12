#include "grbl_settings_list_widget.h"

GrblSettingsListWidget::GrblSettingsListWidget(const Rect& bounds, GrblController& grbl, int16_t rowHeight)
    : Widget(bounds), grbl(grbl), rowHeight(rowHeight)
{
    recalculateMaxScroll();
}

void GrblSettingsListWidget::setOnSelect(SelectCallback callback) { onSelect = callback; }

void GrblSettingsListWidget::recalculateMaxScroll()
{
    int16_t contentHeight = GRBL_SETTINGS_TABLE_COUNT * rowHeight;
    maxScrollOffset = max((int16_t)0, (int16_t)(contentHeight - bounds.height));
}

void GrblSettingsListWidget::clampScroll()
{
    if (scrollOffset < 0) scrollOffset = 0;
    if (scrollOffset > maxScrollOffset) scrollOffset = maxScrollOffset;
}

void GrblSettingsListWidget::refresh()
{
    invalidate();
}

String GrblSettingsListWidget::formatValue(const GrblSettingDef& def, float value) const
{
    if (def.type == GrblSettingType::Toggle)
        return (value > 0.5f) ? "ON" : "OFF";

    if (def.type == GrblSettingType::Enum && def.enumLabels)
    {
        uint8_t idx = (uint8_t)value;
        if (idx < def.enumCount)
            return def.enumLabels[idx];
    }

    String result = String(value, 2);
    if (strlen(def.unit) > 0)
        result += " " + String(def.unit);

    return result;
}

bool GrblSettingsListWidget::handleTouch(const TouchEvent& event)
{
    if (!dragging && !bounds.contains(event.point.x, event.point.y))
        return false;

    switch (event.type)
    {
        case TouchType::Pressed:
            dragging = true;
            lastTouchY = event.point.y;
            totalDragDistance = 0;
            return true;

        case TouchType::Move:
        {
            if (!dragging) return false;

            int16_t delta = lastTouchY - event.point.y;
            scrollOffset += delta;
            clampScroll();

            totalDragDistance += abs(delta);
            lastTouchY = event.point.y;

            invalidate();
            return true;
        }

        case TouchType::Released:
        {
            if (!dragging) return false;
            dragging = false;

            if (totalDragDistance < TAP_THRESHOLD)
            {
                int16_t relativeY = (event.point.y - bounds.y) + scrollOffset;
                uint8_t index = relativeY / rowHeight;

                if (index < GRBL_SETTINGS_TABLE_COUNT && onSelect)
                {
                    const GrblSettingDef& def = GRBL_SETTINGS_TABLE[index];
                    float currentValue = grbl.getSettings().get(def.index, 0.0f);
                    onSelect(def, currentValue);
                }
            }

            return true;
        }

        default:
            return false;
    }
}

void GrblSettingsListWidget::draw(DisplayManager& display)
{
    if (!dirty)
        return;

    display.setClipRect(bounds.x, bounds.y, bounds.width, bounds.height);
    display.fillRect(bounds.x, bounds.y, bounds.width, bounds.height, Theme::Background);

    int16_t firstVisible = scrollOffset / rowHeight;
    int16_t y = bounds.y - (scrollOffset % rowHeight);

    for (int16_t i = firstVisible; i < GRBL_SETTINGS_TABLE_COUNT && y < bounds.y + bounds.height; i++)
    {
        const GrblSettingDef& def = GRBL_SETTINGS_TABLE[i];
        float value = grbl.getSettings().get(def.index, 0.0f);

        // Small index at the top, name + value at the bottom
        display.drawText("$" + String(def.index), bounds.x + 8, y + 4, Theme::TextSecondary, 1);
        display.drawText(def.name, bounds.x + 8, y + 16, Theme::Text, 2);

        String valueText = formatValue(def, value);
        display.drawText(valueText, bounds.x + bounds.width - 90, y + rowHeight / 2 - 8, Theme::Progress, 2);

        display.drawLine(bounds.x, y + rowHeight, bounds.x + bounds.width, y + rowHeight, Theme::Border);

        y += rowHeight;
    }

    display.clearClipRect();

    if (maxScrollOffset > 0)
    {
        constexpr int16_t SCROLLBAR_WIDTH = 3;
        int16_t trackX = bounds.x + bounds.width - SCROLLBAR_WIDTH - 2;

        int16_t thumbHeight = max((int16_t)16, (int16_t)((float)bounds.height * bounds.height / (bounds.height + maxScrollOffset)));
        int16_t thumbY = bounds.y + (int16_t)((float)(bounds.height - thumbHeight) * scrollOffset / maxScrollOffset);

        display.fillRoundRect(trackX, bounds.y, SCROLLBAR_WIDTH, bounds.height, 1, Theme::SidebarBackground);
        display.fillRoundRect(trackX, thumbY, SCROLLBAR_WIDTH, thumbHeight, 1, Theme::SidebarBorderSelected);
    }

    dirty = false;
}