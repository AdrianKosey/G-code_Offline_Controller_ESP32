#include "wifi_network_list_widget.h"
#include "../../../i18n/translations.h"
WifiNetworkListWidget::WifiNetworkListWidget(const Rect& bounds, int16_t rowHeight)
    : Widget(bounds), rowHeight(rowHeight)
{}

void WifiNetworkListWidget::setOnSelect(SelectCallback callback) { onSelect = callback; }

void WifiNetworkListWidget::setEntries(const WifiNetworkEntry* newEntries, uint8_t count)
{
    entryCount = min(count, (uint8_t)MAX_ITEMS);

    for (uint8_t i = 0; i < entryCount; i++)
        entries[i] = newEntries[i];

    scrollOffset = 0;
    recalculateMaxScroll();
    invalidate();
}

void WifiNetworkListWidget::clear()
{
    entryCount = 0;
    scrollOffset = 0;
    invalidate();
}

void WifiNetworkListWidget::recalculateMaxScroll()
{
    int16_t contentHeight = entryCount * rowHeight;
    maxScrollOffset = max((int16_t)0, (int16_t)(contentHeight - bounds.height));
}

void WifiNetworkListWidget::clampScroll()
{
    if (scrollOffset < 0) scrollOffset = 0;
    if (scrollOffset > maxScrollOffset) scrollOffset = maxScrollOffset;
}

bool WifiNetworkListWidget::handleTouch(const TouchEvent& event)
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

                if (index < entryCount && onSelect)
                    onSelect(entries[index]);
            }

            return true;
        }

        default:
            return false;
    }
}

void WifiNetworkListWidget::draw(DisplayManager& display)
{
    if (!dirty)
        return;

    display.setClipRect(bounds.x, bounds.y, bounds.width, bounds.height);
    display.fillRect(bounds.x, bounds.y, bounds.width, bounds.height, Theme::Background);

    int16_t firstVisible = scrollOffset / rowHeight;
    int16_t y = bounds.y - (scrollOffset % rowHeight);

    for (int16_t i = firstVisible; i < entryCount && y < bounds.y + bounds.height; i++)
    {
        const WifiNetworkEntry& entry = entries[i];

        String label = entry.ssid + (entry.encrypted ? tr(StringId::Settings_WiFiProtected) : "");
        display.drawText(label, bounds.x + 8, y + rowHeight / 2 - 8, Theme::Text, 2);
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