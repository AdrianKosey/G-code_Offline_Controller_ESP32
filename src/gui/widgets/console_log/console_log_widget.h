#pragma once

#include <Arduino.h>
#include <vector>
#include "../../core/widget.h"
#include "../../theme.h"
#include "../../../machine/grbl_controller.h"

class ConsoleLogWidget : public Widget
{
public:
    ConsoleLogWidget(const Rect& bounds, int16_t lineHeight = 16);

    void draw(DisplayManager& display) override;
    bool handleTouch(const TouchEvent& event) override;

    // Synchronizes with GrblController's actual history - only performs work if the version has changed
    void syncFrom(const GrblController& grbl);

private:
    static constexpr size_t MAX_LINES = 100;

    std::vector<ConsoleEntry> lines;
    uint32_t lastSyncedVersion = 0xFFFFFFFF; // force sync the first time

    int16_t lineHeight;
    int16_t scrollOffset = 0;
    int16_t maxScrollOffset = 0;

    bool dragging = false;
    int16_t lastTouchY = 0;
    int16_t totalDragDistance = 0;
    bool autoScrollToBottom = true; // while the user has not scrolled manually, stay attached to the bottom

    static constexpr int16_t TAP_THRESHOLD = 8;

    void recalculateMaxScroll();
    void clampScroll();
    uint16_t colorForLine(const String& text) const;
};