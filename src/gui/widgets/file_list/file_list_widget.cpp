#include "file_list_widget.h"

FileListWidget::FileListWidget(const Rect& bounds, int16_t rowHeight)
    : Widget(bounds), rowHeight(rowHeight)
{}

void FileListWidget::setOnSelect(SelectCallback callback)
{
    onSelect = callback;
}

void FileListWidget::setEntries(const FileEntry* newEntries, uint8_t count)
{
    entryCount = min(count, (uint8_t)MAX_ITEMS);

    for (uint8_t i = 0; i < entryCount; i++)
        entries[i] = newEntries[i];

    scrollOffset = 0;
    recalculateMaxScroll();
    invalidate();
}

void FileListWidget::clear()
{
    entryCount = 0;
    scrollOffset = 0;
    invalidate();
}

void FileListWidget::recalculateMaxScroll()
{
    int16_t contentHeight = entryCount * rowHeight;
    maxScrollOffset = max((int16_t)0, (int16_t)(contentHeight - bounds.height));
}

void FileListWidget::clampScroll()
{
    if (scrollOffset < 0) scrollOffset = 0;
    if (scrollOffset > maxScrollOffset) scrollOffset = maxScrollOffset;
}

bool FileListWidget::handleTouch(const TouchEvent& event)
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
            if (!dragging)
                return false;

            int16_t delta = lastTouchY - event.point.y; // finger up = content up (scroll down)
            scrollOffset += delta;
            clampScroll();

            totalDragDistance += abs(delta);
            lastTouchY = event.point.y;

            invalidate();
            return true;
        }

        case TouchType::Released:
        {
            if (!dragging)
                return false;

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

void FileListWidget::draw(DisplayManager& display)
{
    if (!dirty)
        return;

    display.setClipRect(bounds.x, bounds.y, bounds.width, bounds.height);

    display.fillRect(bounds.x, bounds.y, bounds.width, bounds.height, Theme::Background);

    int16_t firstVisible = scrollOffset / rowHeight;
    int16_t y = bounds.y - (scrollOffset % rowHeight);

    for (int16_t i = firstVisible; i < entryCount && y < bounds.y + bounds.height; i++)
    {
        const FileEntry& entry = entries[i];

        uint16_t textColor = (entry.type == FileEntryType::Folder) ? Theme::SidebarBorderSelected : Theme::Text;
        String prefix = (entry.type == FileEntryType::Folder) ? "[ ] " : "";

        display.drawText(prefix + entry.name, bounds.x + 8, y + rowHeight / 2 - 8, textColor, 2);
        display.drawLine(bounds.x, y + rowHeight, bounds.x + bounds.width, y + rowHeight, Theme::Border);

        y += rowHeight;
    }

    display.clearClipRect();

    dirty = false;
}