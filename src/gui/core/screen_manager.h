#pragma once

#include "../widgets/sidebar/sidebar_widget.h"
#include "screen.h"

class ScreenManager
{
public:
    static constexpr uint8_t MAX_SCREENS = 8;

    ScreenManager(DisplayManager& display, const Rect& sidebarBounds, const SidebarItem* items, uint8_t itemCount);

    void registerScreen(uint8_t id, IScreen* screen);

    void draw();

    void update();

    void handleTouch(const TouchEvent& event);

private:
    DisplayManager& display;
    SidebarWidget sidebar;
    IScreen* screens[MAX_SCREENS] = { nullptr };

    uint8_t currentId = 0;

    void setActiveScreen(uint8_t id);
};