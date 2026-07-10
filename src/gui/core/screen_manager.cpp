#include "screen_manager.h"

ScreenManager::ScreenManager(DisplayManager& display, const Rect& sidebarBounds, const SidebarItem* items, uint8_t itemCount)
    : display(display), sidebar(sidebarBounds, items, itemCount)
{
    sidebar.setOnSelect([this](uint8_t id) { setActiveScreen(id); });
}

void ScreenManager::registerScreen(uint8_t id, IScreen* screen)
{
    if (id < MAX_SCREENS)
        screens[id] = screen;
}

void ScreenManager::setActiveScreen(uint8_t id)
{
    if (id >= MAX_SCREENS || screens[id] == nullptr || id == currentId)
        return;

    if (screens[currentId])
        screens[currentId]->onExit();

    currentId = id;

    display.fillRect(
        sidebar.getBounds().width,
        0,
        display.width() - sidebar.getBounds().width,
        display.height(),
        Theme::Background);

    screens[currentId]->onEnter();
}

void ScreenManager::draw()
{
    sidebar.draw(display);

    if (screens[currentId])
        screens[currentId]->draw(display);
}

void ScreenManager::update()
{
    if (screens[currentId])
        screens[currentId]->update();
}

void ScreenManager::handleTouch(const TouchEvent& event)
{
    if (sidebar.handleTouch(event))
        return;

    if (screens[currentId])
        screens[currentId]->handleTouch(event);
}