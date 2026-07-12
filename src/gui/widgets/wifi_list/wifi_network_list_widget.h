#pragma once

#include <Arduino.h>
#include <functional>
#include "../../core/widget.h"
#include "../../theme.h"

struct WifiNetworkEntry
{
    String ssid;
    int32_t rssi;
    bool encrypted;
};

class WifiNetworkListWidget : public Widget
{
public:
    using SelectCallback = std::function<void(const WifiNetworkEntry&)>;

    static constexpr uint8_t MAX_ITEMS = 20;

    WifiNetworkListWidget(const Rect& bounds, int16_t rowHeight = 34);

    void draw(DisplayManager& display) override;
    bool handleTouch(const TouchEvent& event) override;

    void setEntries(const WifiNetworkEntry* entries, uint8_t count);
    void clear();

    void setOnSelect(SelectCallback callback);

private:
    WifiNetworkEntry entries[MAX_ITEMS];
    uint8_t entryCount = 0;

    int16_t rowHeight;
    int16_t scrollOffset = 0;
    int16_t maxScrollOffset = 0;

    bool dragging = false;
    int16_t lastTouchY = 0;
    int16_t totalDragDistance = 0;

    SelectCallback onSelect;

    static constexpr int16_t TAP_THRESHOLD = 8;

    void recalculateMaxScroll();
    void clampScroll();
};