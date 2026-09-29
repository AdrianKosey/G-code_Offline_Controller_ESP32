#include "console_log_widget.h"

ConsoleLogWidget::ConsoleLogWidget(const Rect& bounds, int16_t lineHeight)
    : Widget(bounds), lineHeight(lineHeight)
{}

void ConsoleLogWidget::syncFrom(const GrblController& grbl)
{
    uint32_t currentVersion = grbl.getConsoleVersion();

    if (currentVersion == lastSyncedVersion)
        return;

    lastSyncedVersion = currentVersion;

    const auto& history = grbl.getConsoleHistory();
    lines.assign(history.begin(), history.end());

    if (lines.size() > MAX_LINES)
        lines.erase(lines.begin(), lines.end() - MAX_LINES);

    recalculateMaxScroll();

    if (autoScrollToBottom)
        scrollOffset = maxScrollOffset;

    invalidate();
}

void ConsoleLogWidget::recalculateMaxScroll()
{
    int16_t contentHeight = (int16_t)lines.size() * lineHeight;
    maxScrollOffset = max((int16_t)0, (int16_t)(contentHeight - bounds.height));
}

void ConsoleLogWidget::clampScroll()
{
    if (scrollOffset < 0) scrollOffset = 0;
    if (scrollOffset > maxScrollOffset) scrollOffset = maxScrollOffset;
}

uint16_t ConsoleLogWidget::colorForLine(const String& text) const
{
    String upper = text;
    upper.toUpperCase();

    if (upper.startsWith("ALARM")) return Theme::Error;
    if (upper.startsWith("ERROR")) return Theme::Error;
    if (upper.startsWith("[MSG:")) return Theme::Progress;
    if (upper == "OK") return Theme::Success;

    // Look for the first word of the G0/G1/M3/M5 etc. type as a complete token.
    int start = 0;
    while (start < (int)upper.length())
    {
        int end = upper.indexOf(' ', start);
        if (end < 0) end = upper.length();

        String token = upper.substring(start, end);

        if (token == "G0" || token == "G00")
            return Theme::TextSecondary; // Quick – gray, it's not real work.

        if (token == "G1" || token == "G01" || token == "G2" || token == "G02" || token == "G3" || token == "G03")
            return Theme::Progress3; // actual cutting motion

        if (token == "M3" || token == "M03" || token == "M4" || token == "M04")
            return Theme::Warning; // spindle/laser turning on

        if (token == "M5" || token == "M05")
            return Theme::TextSecondary;

        start = end + 1;
    }

    return Theme::Text;
}

bool ConsoleLogWidget::handleTouch(const TouchEvent& event)
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

            // If the user scrolls manually, stop auto-snapping to the bottom -
            // only resume auto-scrolling if they scroll all the way down themselves.
            autoScrollToBottom = (scrollOffset >= maxScrollOffset - 2);

            invalidate();
            return true;
        }

        case TouchType::Released:
            dragging = false;
            return true;

        default:
            return false;
    }
}

void ConsoleLogWidget::draw(DisplayManager& display)
{
    if (!dirty)
        return;

    display.setClipRect(bounds.x, bounds.y, bounds.width, bounds.height);
    display.fillRect(bounds.x, bounds.y, bounds.width, bounds.height, Theme::Background);

    int16_t firstVisible = scrollOffset / lineHeight;
    int16_t y = bounds.y - (scrollOffset % lineHeight);

    for (size_t i = firstVisible; i < lines.size() && y < bounds.y + bounds.height; i++)
    {
        const ConsoleEntry& entry = lines[i];
        String prefix = entry.isOutgoing ? "> " : "< ";
        uint16_t color = colorForLine(entry.text);

        display.drawText(prefix + entry.text, bounds.x + 4, y + 1, color, 1);
        y += lineHeight;
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