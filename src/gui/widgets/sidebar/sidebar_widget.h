#pragma once

#include <Arduino.h>
#include <functional>

#include "../../core/widget.h"
#include "../../theme.h"

enum class IconId
{
    Home,
    Files,
    Prepare,
    Tools,
    Settings
};

struct SidebarItem
{
    IconId icon;
    const char* label;
};

class SidebarWidget : public Widget
{
public:
    using SelectCallback = std::function<void(uint8_t)>;

    SidebarWidget(const Rect& bounds, const SidebarItem* items, uint8_t count);

    void draw(DisplayManager& display) override;

    bool handleTouch(const TouchEvent& event) override;

    void setSelectedIndex(uint8_t index);

    uint8_t getSelectedIndex() const;

    void setOnSelect(SelectCallback callback);

private:
    const SidebarItem* items;
    uint8_t count;
    uint8_t selectedIndex = 0;

    SelectCallback onSelect;

    int16_t itemHeight() const;

    void drawIcon(DisplayManager& display, IconId icon, int16_t cx, int16_t cy, uint16_t color) const;
};