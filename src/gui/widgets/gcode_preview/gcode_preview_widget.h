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
    static constexpr uint16_t MAX_POINTS = 400; // I don't know how much available memory is sufficient.

    GCodePreviewWidget(
        const Rect& bounds,
        uint16_t backgroundColor = Theme::Background,
        uint16_t pathColor = Theme::TextSecondary,
        uint16_t cursorColor = Theme::Text);

    void draw(DisplayManager& display) override;

    // Defines the actual area (in mm) that the project occupies - called ONCE when loading the file
    void setProjectBounds(float minX, float minY, float maxX, float maxY);

    // Clear the drawn path (to load a new project)
    void clearPath();

    // Add a point to the path taken. The caller decides the sampling/downsampling.
    void addPoint(float x, float y);

    // Current position of the tool (the white cross)
    void setCursor(float x, float y);

private:
    uint16_t backgroundColor;
    uint16_t pathColor;
    uint16_t cursorColor;

    // Project limits in mm
    float minX = 0, minY = 0, maxX = 0, maxY = 0;

    // Scale factor and offset already calculated (mm -> pixels)
    float scale = 1.0f;
    int16_t offsetX = 0;
    int16_t offsetY = 0;

    GCodePoint points[MAX_POINTS];
    uint16_t pointCount = 0;

    float cursorX = 0;
    float cursorY = 0;

    static constexpr int16_t PADDING = 4; // inner margin so that the stroke does not touch the edge
    void recalculateScale();

    // Converts a project coordinate (mm) to pixel within the widget
    void worldToScreen(float x, float y, int16_t& screenX, int16_t& screenY) const;
};