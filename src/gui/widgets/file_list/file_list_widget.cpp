#include "file_list_widget.h"

FileListWidget::FileListWidget(const Rect &bounds, int16_t rowHeight)
    : Widget(bounds), rowHeight(rowHeight)
{
}

void FileListWidget::setOnSelect(SelectCallback callback)
{
    onSelect = callback;
}

void FileListWidget::setEntries(const FileEntry *newEntries, uint8_t count)
{
    entryCount = min(count, (uint8_t)MAX_ITEMS);

    for (uint8_t i = 0; i < entryCount; i++)
        entries[i] = newEntries[i];

    scrollOffset = 0;
    inertiaActive = false;
    recalculateMaxScroll();
    invalidate();
}

void FileListWidget::clear()
{
    entryCount = 0;
    scrollOffset = 0;
    inertiaActive = false;
    invalidate();
}

void FileListWidget::recalculateMaxScroll()
{
    int16_t contentHeight = entryCount * rowHeight;
    maxScrollOffset = max((int16_t)0, (int16_t)(contentHeight - bounds.height));
}

void FileListWidget::clampScroll()
{
    if (scrollOffset < 0)
        scrollOffset = 0;
    if (scrollOffset > maxScrollOffset)
        scrollOffset = maxScrollOffset;
}

bool FileListWidget::handleTouch(const TouchEvent &event)
{
    if (!dragging && !bounds.contains(event.point.x, event.point.y))
        return false;

    switch (event.type)
    {
    case TouchType::Pressed:
        dragging = true;
        inertiaActive = false;
        velocity = 0;
        lastTouchY = event.point.y;
        totalDragDistance = 0;
        pressStartTime = millis();
        lastMoveTime = pressStartTime;
        return true;

    case TouchType::Move:
    {
        if (!dragging)
            return false;

        unsigned long now = millis();
        unsigned long dt = now - lastMoveTime;

        int16_t delta = lastTouchY - event.point.y;
        scrollOffset += delta;
        clampScroll();

        if (dt > 0)
            velocity = (float)delta * 16.0f / (float)dt; // normalizado a ~16ms/frame

        totalDragDistance += abs(delta);
        lastTouchY = event.point.y;
        lastMoveTime = now;

        invalidate();
        return true;
    }

    case TouchType::Released:
    {
        if (!dragging)
            return false;

        dragging = false;

        bool wasTap = totalDragDistance < TAP_THRESHOLD &&
                      (millis() - pressStartTime) > MIN_TAP_DURATION_MS;

        if (wasTap)
        {
            int16_t relativeY = (event.point.y - bounds.y) + scrollOffset;
            uint8_t index = relativeY / rowHeight;

            if (index < entryCount && onSelect)
                onSelect(entries[index]);
        }
        else if (abs(velocity) > MIN_VELOCITY)
        {
            inertiaActive = true;
        }

        return true;
    }

    default:
        return false;
    }
}

void FileListWidget::update()
{
    if (!inertiaActive)
        return;

    scrollOffset += (int16_t)velocity;
    velocity *= FRICTION;

    bool hitEdge = scrollOffset <= 0 || scrollOffset >= maxScrollOffset;
    clampScroll();

    if (hitEdge || abs(velocity) < MIN_VELOCITY)
        inertiaActive = false;

    invalidate();
}

void FileListWidget::draw(DisplayManager &display)
{
    if (!dirty)
        return;

    display.setClipRect(bounds.x, bounds.y, bounds.width, bounds.height);
    display.fillRect(bounds.x, bounds.y, bounds.width, bounds.height, Theme::Background);

    int16_t firstVisible = scrollOffset / rowHeight;
    int16_t y = bounds.y - (scrollOffset % rowHeight);

    for (int16_t i = firstVisible; i < entryCount && y < bounds.y + bounds.height; i++)
    {
        const FileEntry &entry = entries[i];

        uint16_t textColor = (entry.type == FileEntryType::Folder) ? Theme::SidebarBorderSelected : Theme::Text;
        const uint8_t *icon = (entry.type == FileEntryType::Folder) ? Icons::Folder : Icons::FileIcon;

        int16_t iconY = y + rowHeight / 2 - Icons::FILE_ICON_HEIGHT / 2;
        display.drawBitmap(bounds.x + 6, iconY, icon, Icons::FILE_ICON_WIDTH, Icons::FILE_ICON_HEIGHT, textColor);

        int16_t textX = bounds.x + 6 + Icons::FILE_ICON_WIDTH + 6;
        display.drawText(entry.name, textX, y + rowHeight / 2 - 8, textColor, 2);

        display.drawLine(bounds.x, y + rowHeight, bounds.x + bounds.width, y + rowHeight, Theme::Border);

        y += rowHeight;
    }

    display.clearClipRect();

    if (maxScrollOffset > 0)
    {
        constexpr int16_t SCROLLBAR_WIDTH = 3;
        int16_t trackX = bounds.x + bounds.width - SCROLLBAR_WIDTH - 2;

        int16_t thumbHeight = max(
            (int16_t)16,
            (int16_t)((float)bounds.height * bounds.height / (bounds.height + maxScrollOffset)));

        int16_t thumbY = bounds.y +
                         (int16_t)((float)(bounds.height - thumbHeight) * scrollOffset / maxScrollOffset);

        display.fillRoundRect(trackX, bounds.y, SCROLLBAR_WIDTH, bounds.height, 1, Theme::SidebarBackground);
        display.fillRoundRect(trackX, thumbY, SCROLLBAR_WIDTH, thumbHeight, 1, Theme::SidebarBorderSelected);
    }

    dirty = false;
}