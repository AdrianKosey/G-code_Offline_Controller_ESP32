#include "sidebar_widget.h"
#include "../../icons.h"
SidebarWidget::SidebarWidget(const Rect& bounds, const SidebarItem* items, uint8_t count)
    : Widget(bounds), items(items), count(count)
{}

int16_t SidebarWidget::itemHeight() const
{
    return bounds.height / count;
}

void SidebarWidget::setOnSelect(SelectCallback callback)
{
    onSelect = callback;
}

void SidebarWidget::setSelectedIndex(uint8_t index)
{
    if (index >= count || index == selectedIndex)
        return;

    selectedIndex = index;
    invalidate();
}

uint8_t SidebarWidget::getSelectedIndex() const
{
    return selectedIndex;
}

bool SidebarWidget::handleTouch(const TouchEvent& event)
{
    if (event.type != TouchType::Pressed)
        return false;

    if (!bounds.contains(event.point.x, event.point.y))
        return false;

    uint8_t index = (event.point.y - bounds.y) / itemHeight();

    if (index >= count)
        return false;

    if (index != selectedIndex)
    {
        selectedIndex = index;
        invalidate();

        if (onSelect)
            onSelect(selectedIndex);
    }

    return true; // The sidebar always "consumes" the touch within its bounds
}

void SidebarWidget::draw(DisplayManager& display)
{
    if (!dirty)
        return;

    int16_t h = itemHeight();

    for (uint8_t i = 0; i < count; i++)
    {
        int16_t itemY = bounds.y + i * h;
        bool selected = (i == selectedIndex);

        uint16_t bg = selected ? Theme::SidebarSelected : Theme::SidebarBackground;

        display.fillRect(bounds.x, itemY, bounds.width, h, bg);

        if (selected)
        {
            // accent bar on the left
            display.fillRect(bounds.x, itemY, 4, h, Theme::SidebarBorderSelected);
        }

        uint16_t iconColor = selected ? Theme::SidebarBorderSelected : Theme::TextSecondary;

        drawIcon(display, items[i].icon, bounds.x + bounds.width / 2, itemY + h / 3, iconColor);

        display.drawText(
            items[i].label,
            bounds.x + 8,
            itemY + (1.5 * h) / 3 + 10,
            selected ? Theme::Text : Theme::TextSecondary,
            1);
    }

    dirty = false;
}

void SidebarWidget::drawIcon(DisplayManager& display, IconId icon, int16_t cx, int16_t cy, uint16_t color) const
{
    const unsigned char* bitmap = nullptr;

    switch (icon)
    {
        case IconId::Home:     bitmap = Icons::Home;     break;
        case IconId::Files:    bitmap = Icons::Files;    break;
        case IconId::Prepare:  bitmap = Icons::Prepare;  break;
        case IconId::Tools:    bitmap = Icons::Tools;    break;
        case IconId::Settings: bitmap = Icons::Settings; break;
    }

    if (!bitmap)
        return;

    display.drawBitmap(
        cx - Icons::SIDEBAR_WIDTH / 2,
        cy - Icons::SIDEBAR_HEIGHT / 2,
        bitmap,
        Icons::SIDEBAR_WIDTH,
        Icons::SIDEBAR_HEIGHT,
        color);
}