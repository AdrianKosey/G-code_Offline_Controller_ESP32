#include "gcode_preview_widget.h"

GCodePreviewWidget::GCodePreviewWidget(
    const Rect& bounds,
    uint16_t backgroundColor,
    uint16_t pathColor,
    uint16_t cursorColor)
    : Widget(bounds),
      backgroundColor(backgroundColor),
      pathColor(pathColor),
      cursorColor(cursorColor)
{}

void GCodePreviewWidget::setProjectBounds(float newMinX, float newMinY, float newMaxX, float newMaxY)
{
    minX = newMinX;
    minY = newMinY;
    maxX = newMaxX;
    maxY = newMaxY;

    recalculateScale();
    invalidate();
}

void GCodePreviewWidget::recalculateScale()
{
    float projectWidth = maxX - minX;
    float projectHeight = maxY - minY;

    if (projectWidth <= 0 || projectHeight <= 0)
    {
        scale = 1.0f;
        return;
    }

    int16_t availableWidth = bounds.width - (PADDING * 2);
    int16_t availableHeight = bounds.height - (PADDING * 2);

    float scaleX = availableWidth / projectWidth;
    float scaleY = availableHeight / projectHeight;

    //The smaller of the two, so the project ALWAYS fits completely, without being distorted
    scale = min(scaleX, scaleY);

    // Center the drawing within the available space
    float drawnWidth = projectWidth * scale;
    float drawnHeight = projectHeight * scale;

    offsetX = (int16_t)((availableWidth - drawnWidth) / 2.0f);
    offsetY = (int16_t)((availableHeight - drawnHeight) / 2.0f);
}

void GCodePreviewWidget::worldToScreen(float x, float y, int16_t& screenX, int16_t& screenY) const
{
    screenX = bounds.x + PADDING + offsetX + (int16_t)((x - minX) * scale);

    // Y reversed: in G-code Y grows upwards, on screen it grows downwards
    screenY = bounds.y + PADDING + offsetY + (int16_t)((maxY - y) * scale);
}

void GCodePreviewWidget::clearPath()
{
    pointCount = 0;
    invalidate();
}

void GCodePreviewWidget::addPoint(float x, float y)
{
    if (pointCount >= MAX_POINTS)
        return; // buffer full - caller should be sampling to avoid reaching here

    points[pointCount++] = { x, y };
    invalidate();
}

void GCodePreviewWidget::setCursor(float x, float y)
{
    cursorX = x;
    cursorY = y;
    invalidate();
}

void GCodePreviewWidget::draw(DisplayManager& display)
{
    if (!dirty)
        return;

    display.fillRect(bounds.x, bounds.y, bounds.width, bounds.height, backgroundColor);
    display.drawRect(bounds.x, bounds.y, bounds.width, bounds.height, Theme::Border);

    int16_t x1, y1, x2, y2;

    for (uint16_t i = 1; i < pointCount; i++)
    {
        worldToScreen(points[i - 1].x, points[i - 1].y, x1, y1);
        worldToScreen(points[i].x, points[i].y, x2, y2);

        display.drawLine(x1, y1, x2, y2, pathColor);
    }

    // Cursor: white cross over the current position of the tool
    int16_t cx, cy;
    worldToScreen(cursorX, cursorY, cx, cy);

    constexpr int16_t CURSOR_SIZE = 4;

    display.drawLine(cx - CURSOR_SIZE, cy, cx + CURSOR_SIZE, cy, cursorColor);
    display.drawLine(cx, cy - CURSOR_SIZE, cx, cy + CURSOR_SIZE, cursorColor);

    dirty = false;
}