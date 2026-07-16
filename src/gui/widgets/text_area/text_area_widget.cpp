#include "text_area_widget.h"

TextAreaWidget::TextAreaWidget(const Rect& bounds, uint16_t color, uint8_t font, uint16_t backgroundColor)
    : Widget(bounds), color(color), font(font), backgroundColor(backgroundColor)
{
    lineHeight = 8 * font + 4;
}

void TextAreaWidget::setText(const String& text)
{
    rawText = text;
    wrapped = false;
    scrollOffset = 0;
    invalidate();
}

void TextAreaWidget::wrapText(DisplayManager& display)
{
    lines.clear();

    int16_t maxWidth = bounds.width - 12;
    String currentLine;
    String word;

    auto flushWord = [&]()
    {
        if (word.length() == 0)
            return;

        String candidate = currentLine.length() ? (currentLine + " " + word) : word;

        if (display.getTextWidth(candidate, font) > maxWidth && currentLine.length() > 0)
        {
            lines.push_back(currentLine);
            currentLine = word;
        }
        else
        {
            currentLine = candidate;
        }

        word = "";
    };

    for (uint16_t i = 0; i < rawText.length(); i++)
    {
        char c = rawText[i];

        if (c == '\n')
        {
            flushWord();
            lines.push_back(currentLine);
            currentLine = "";
        }
        else if (c == ' ')
        {
            flushWord();
        }
        else
        {
            word += c;
        }
    }

    flushWord();
    if (currentLine.length() > 0)
        lines.push_back(currentLine);

    wrapped = true;

    int16_t contentHeight = lines.size() * lineHeight;
    maxScrollOffset = max((int16_t)0, (int16_t)(contentHeight - bounds.height));
}

int16_t TextAreaWidget::measure(DisplayManager& display)
{
    if (!wrapped)
        wrapText(display);

    return lines.size() * lineHeight;
}

void TextAreaWidget::clampScroll()
{
    if (scrollOffset < 0) scrollOffset = 0;
    if (scrollOffset > maxScrollOffset) scrollOffset = maxScrollOffset;
}

bool TextAreaWidget::handleTouch(const TouchEvent& event)
{
    if (maxScrollOffset <= 0)
        return false; // There's nothing to scroll, it doesn't use up touch time

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
            dragging = false;
            return true;

        default:
            return false;
    }
}

void TextAreaWidget::draw(DisplayManager& display)
{
    if (!wrapped)
        wrapText(display);

    if (!dirty)
        return;

    display.setClipRect(bounds.x, bounds.y, bounds.width, bounds.height);
    display.fillRect(bounds.x, bounds.y, bounds.width, bounds.height, backgroundColor);

    int16_t firstVisible = scrollOffset / lineHeight;
    int16_t y = bounds.y - (scrollOffset % lineHeight);

    for (size_t i = firstVisible; i < lines.size() && y < bounds.y + bounds.height; i++)
    {
        display.drawText(lines[i], bounds.x + 6, y + 2, color, font);
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