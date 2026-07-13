#include "scroll_panel_widget.h"

ScrollPanelWidget::ScrollPanelWidget(const Rect& bounds) : Widget(bounds) {}

void ScrollPanelWidget::addChild(Widget* child, int16_t relativeX, int16_t relativeY)
{
    children.push_back({ child, relativeX, relativeY });

    int16_t bottom = relativeY + child->getBounds().height;
    if (bottom > contentHeight)
        contentHeight = bottom;

    recalculateMaxScroll();
    repositionChildren();
}

int16_t ScrollPanelWidget::getContentHeight() const { return contentHeight; }

void ScrollPanelWidget::recalculateMaxScroll()
{
    maxScrollOffset = max((int16_t)0, (int16_t)(contentHeight - bounds.height));
}

void ScrollPanelWidget::clampScroll()
{
    if (scrollOffset < 0) scrollOffset = 0;
    if (scrollOffset > maxScrollOffset) scrollOffset = maxScrollOffset;
}

void ScrollPanelWidget::repositionChildren()
{
    for (auto& entry : children)
    {
        entry.widget->setPosition(bounds.x + entry.relativeX, bounds.y + entry.relativeY - scrollOffset);
    }
}

void ScrollPanelWidget::update()
{
    for (auto& entry : children)
        entry.widget->update();
}

bool ScrollPanelWidget::handleTouch(const TouchEvent& event)
{
    if (!dragging && !bounds.contains(event.point.x, event.point.y))
        return false;

    switch (event.type)
    {
        case TouchType::Pressed:
            dragging = true;
            lastTouchY = event.point.y;
            totalDragDistance = 0;
            break;

        case TouchType::Move:
        {
            if (!dragging) return false;

            int16_t delta = lastTouchY - event.point.y;
            scrollOffset += delta;
            clampScroll();

            totalDragDistance += abs(delta);
            lastTouchY = event.point.y;

            repositionChildren();
            invalidate();
            return true;
        }

        case TouchType::Released:
            dragging = false;
            break;

        default:
            return false;
    }

    // I only let children receive the touch if it was NOT a meaningful drag (i.e., a real touch)
    if (totalDragDistance < TAP_THRESHOLD)
    {
        for (auto& entry : children)
        {
            if (entry.widget->handleTouch(event))
                return true;
        }
    }

    return true;
}

void ScrollPanelWidget::draw(DisplayManager& display)
{
    if (!dirty)
        return;

    display.setClipRect(bounds.x, bounds.y, bounds.width, bounds.height);
    display.fillRect(bounds.x, bounds.y, bounds.width, bounds.height, Theme::Background);

    for (auto& entry : children)
    {
        int16_t childBottom = entry.widget->getBounds().y + entry.widget->getBounds().height;

        if (childBottom < bounds.y || entry.widget->getBounds().y > bounds.y + bounds.height)
            continue;

        entry.widget->invalidate();
        entry.widget->draw(display);
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