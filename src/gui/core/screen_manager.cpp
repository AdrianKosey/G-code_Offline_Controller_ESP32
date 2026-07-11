#include "screen_manager.h"

ScreenManager::ScreenManager(
    const Rect& sidebarBounds,
    const SidebarItem* items,
    uint8_t itemCount,
    const Rect& headerBounds)
    : sidebar(sidebarBounds, items, itemCount),
      header(headerBounds),
      sidebarBounds(sidebarBounds),
      headerBounds(headerBounds)
{
    sidebar.setOnSelect([this](uint8_t id) { setActiveScreen(id); });
}

void ScreenManager::registerScreen(uint8_t id, IScreen* screen, const String& title)
{
    if (id >= MAX_SCREENS)
        return;

    screens[id].screen = screen;
    screens[id].title = title;
}

void ScreenManager::setActiveScreen(uint8_t id)
{
    if (id >= MAX_SCREENS || screens[id].screen == nullptr || id == currentId)
        return;

    if (screens[currentId].screen)
        screens[currentId].screen->onExit();

    currentId = id;

    if (display)
    {
        display->fillRect(
            sidebarBounds.width,
            headerBounds.height,
            display->width() - sidebarBounds.width,
            display->height() - headerBounds.height,
            Theme::Background);
    }

    header.setTitle(screens[currentId].title);
    screens[currentId].screen->onEnter();
}

void ScreenManager::draw(DisplayManager& displayRef)
{
    display = &displayRef;

    header.draw(displayRef);
    sidebar.draw(displayRef);

    if (screens[currentId].screen)
        screens[currentId].screen->draw(displayRef);
}

void ScreenManager::update()
{
    if (screens[currentId].screen)
        screens[currentId].screen->update();
}

void ScreenManager::handleTouch(const TouchEvent& event)
{
    if (sidebar.handleTouch(event))
        return;

    if (screens[currentId].screen)
        screens[currentId].screen->handleTouch(event);
}

void ScreenManager::setSdStatus(bool ready)
{
    header.setSdStatus(ready);
}

void ScreenManager::setWifiStatus(bool connected)
{
    header.setWifiStatus(connected);
}
void ScreenManager::showInitialScreen(uint8_t id)
{
    if (id >= MAX_SCREENS || screens[id].screen == nullptr)
        return;

    currentId = id;
    header.setTitle(screens[id].title);
    screens[id].screen->onEnter();
}

void ScreenManager::switchToScreen(uint8_t id)
{
    sidebar.setSelectedIndex(id); 
    setActiveScreen(id);
}

void ScreenManager::invalidateAll()
{
    sidebar.invalidate(); 
    header.invalidate();

    if (screens[currentId].screen)
        screens[currentId].screen->invalidateAll();
}

void ScreenManager::redrawAll()
{
    if (display)
    {

        display->fillRect(0, 0, display->width(), display->height(), Theme::Background);
    }

    invalidateAll();
}