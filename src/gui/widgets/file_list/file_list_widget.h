#pragma once

#include <Arduino.h>
#include <functional>
#include "../../core/widget.h"
#include "../../theme.h"

enum class FileEntryType
{
    File,
    Folder
};

struct FileEntry
{
    String name;
    FileEntryType type;
};

class FileListWidget : public Widget
{
public:
    using SelectCallback = std::function<void(const FileEntry&)>;

    static constexpr uint8_t MAX_ITEMS = 255; // The maximum number of files expected per folder

    FileListWidget(const Rect& bounds, int16_t rowHeight = 28);

    void draw(DisplayManager& display) override;
    bool handleTouch(const TouchEvent& event) override;

    void setEntries(const FileEntry* entries, uint8_t count);
    void clear();

    void setOnSelect(SelectCallback callback);

private:
    FileEntry entries[MAX_ITEMS];
    uint8_t entryCount = 0;

    int16_t rowHeight;
    int16_t scrollOffset = 0;
    int16_t maxScrollOffset = 0;

    bool dragging = false;
    int16_t lastTouchY = 0;
    int16_t totalDragDistance = 0;

    SelectCallback onSelect;

    static constexpr int16_t TAP_THRESHOLD = 8; // px - below this is tap, not scroll

    void recalculateMaxScroll();
    void clampScroll();
};