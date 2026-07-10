#include "display_manager.h"

DisplayManager::DisplayManager(IDisplayDriver& driver)
    : driver(driver)
{
}

bool DisplayManager::begin()
{
    return driver.begin();
}

void DisplayManager::update()
{
    // Update display logic here
}

void DisplayManager::drawText(const String& text, int16_t x, int16_t y, uint16_t color, uint8_t font)
{
    // Draw text logic here
}

void DisplayManager::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    // Draw pixel logic here
}

void DisplayManager::clear(uint16_t color)
{
    driver.clear(color);
}

void DisplayManager::setRotation(uint8_t rotation)
{
    // Set rotation logic here
}