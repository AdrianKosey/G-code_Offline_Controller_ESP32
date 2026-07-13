#pragma once

#include <vector>
#include "widget.h"
#include "../theme.h"

class ScrollPanelWidget : public Widget
{
public:
    ScrollPanelWidget(const Rect& bounds);

    void addChild(Widget* child, int16_t relativeX, int16_t relativeY);

    void draw(DisplayManager& display) override;
    void update() override;
    bool handleTouch(const TouchEvent& event) override;

    int16_t getContentHeight() const;

private:
    struct ChildEntry
    {
        Widget* widget;
        int16_t relativeX;
        int16_t relativeY;
    };

    std::vector<ChildEntry> children;

    int16_t scrollOffset = 0;
    int16_t maxScrollOffset = 0;
    int16_t contentHeight = 0;

    bool dragging = false;
    int16_t lastTouchY = 0;
    int16_t totalDragDistance = 0;

    static constexpr int16_t TAP_THRESHOLD = 8;

    void repositionChildren();
    void recalculateMaxScroll();
    void clampScroll();
};