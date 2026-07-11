#pragma once

#include <Arduino.h>
#include "../../core/widget.h"
#include "../../theme.h"

struct GCodePoint
{
    float x;
    float y;
};

class GCodePreviewWidget : public Widget
{
public:
    static constexpr uint16_t MAX_POINTS = 400;

    GCodePreviewWidget(
        const Rect& bounds,
        uint16_t backgroundColor = Theme::Background,
        uint16_t pathColor = Theme::TextSecondary,
        uint16_t cursorColor = Theme::Text,
        uint16_t traveledColor = Theme::Progress);

    void draw(DisplayManager& display) override;

    void setProjectBounds(float minX, float minY, float maxX, float maxY);
    void clearPath();
    void addPoint(float x, float y);
    void setCursor(float x, float y);

    void setProgress(float percent); 

private:
    uint16_t backgroundColor;
    uint16_t pathColor;
    uint16_t cursorColor;
    uint16_t traveledColor;

    float minX = 0, minY = 0, maxX = 0, maxY = 0;

    float scale = 1.0f;
    int16_t offsetX = 0;
    int16_t offsetY = 0;

    GCodePoint points[MAX_POINTS];
    uint16_t pointCount = 0;
    uint16_t traveledCount = 0;

    float cursorX = 0;
    float cursorY = 0;

    static constexpr int16_t PADDING = 4;

    void recalculateScale();
    void worldToScreen(float x, float y, int16_t& screenX, int16_t& screenY) const;
};