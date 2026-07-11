#pragma once

#include <Arduino.h>
#include <functional>
#include "../../core/widget.h"
#include "../../theme.h"
#include "../../icons.h"
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

    static constexpr uint8_t MAX_ITEMS = 64;

    FileListWidget(const Rect& bounds, int16_t rowHeight = 28);

    void draw(DisplayManager& display) override;
    void update() override;
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
    unsigned long pressStartTime = 0;

    float velocity = 0;
    bool inertiaActive = false;
    unsigned long lastMoveTime = 0;

    SelectCallback onSelect;

    static constexpr int16_t TAP_THRESHOLD = 8;
    static constexpr unsigned long MIN_TAP_DURATION_MS = 30;
    static constexpr float FRICTION = 0.92f;
    static constexpr float MIN_VELOCITY = 0.5f;

    void recalculateMaxScroll();
    void clampScroll();
};