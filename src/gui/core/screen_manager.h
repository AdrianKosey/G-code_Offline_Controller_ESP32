#pragma once

#include "../widgets/sidebar/sidebar_widget.h"
#include "header_widget.h"
#include "screen.h"

class ScreenManager
{
public:
    static constexpr uint8_t MAX_SCREENS = 8;

    ScreenManager(
        const Rect& sidebarBounds,
        const SidebarItem* items,
        uint8_t itemCount,
        const Rect& headerBounds);

    void registerScreen(uint8_t id, IScreen* screen, const String& title);

    void draw(DisplayManager& display);
    void update();
    void handleTouch(const TouchEvent& event);

    void setSdStatus(bool ready);
    void setWifiStatus(bool connected);
    void setMachineStatus(bool connected);
    void showInitialScreen(uint8_t id);
    void switchToScreen(uint8_t id);

    void invalidateAll();
    void redrawAll();
private:
    struct ScreenEntry
    {
        IScreen* screen = nullptr;
        String title;
    };

    SidebarWidget sidebar;
    HeaderWidget header;

    Rect sidebarBounds;
    Rect headerBounds;

    ScreenEntry screens[MAX_SCREENS];
    uint8_t currentId = 0;

    DisplayManager* display = nullptr;

    void setActiveScreen(uint8_t id);
};