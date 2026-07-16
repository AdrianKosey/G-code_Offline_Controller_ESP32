#pragma once

#include <Arduino.h>
#include <vector>
#include "../../core/widget.h"
#include "../../theme.h"

class TextAreaWidget : public Widget
{
public:
    TextAreaWidget(
        const Rect& bounds,
        uint16_t color = Theme::Text,
        uint8_t font = 2,
        uint16_t backgroundColor = Theme::Panel);

    void draw(DisplayManager& display) override;
    bool handleTouch(const TouchEvent& event) override;

    void setText(const String& text);

    // How much space does the wrapped text occupy (in pixels high) - requires having previously called
// measure() with a real DisplayManager
    int16_t measure(DisplayManager& display); // Wraps the text and returns the total height in px

private:
    String rawText;
    std::vector<String> lines;
    bool wrapped = false;

    uint16_t color;
    uint8_t font;
    uint16_t backgroundColor;

    int16_t lineHeight;
    int16_t scrollOffset = 0;
    int16_t maxScrollOffset = 0;

    bool dragging = false;
    int16_t lastTouchY = 0;
    int16_t totalDragDistance = 0;

    static constexpr int16_t TAP_THRESHOLD = 8;

    void wrapText(DisplayManager& display);
    void clampScroll();
};