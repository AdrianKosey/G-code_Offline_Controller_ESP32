#include "display_manager.h"

DisplayManager::DisplayManager(IDisplayDriver &driver)
    : driver(driver)
{
}

bool DisplayManager::begin()
{
    driver.begin();
    driver.setRotation(1);

    driver.clear(0x0000);

    return true;
}

void DisplayManager::update()
{
}

void DisplayManager::clear(uint16_t color)
{
    driver.clear(color);
}

void DisplayManager::setRotation(uint8_t rotation)
{
    driver.setRotation(rotation);
}

uint16_t DisplayManager::width() const
{
    return driver.width();
}

uint16_t DisplayManager::height() const
{
    return driver.height();
}

void DisplayManager::drawText(
    const String &text,
    int16_t x,
    int16_t y,
    uint16_t color,
    uint8_t font)
{
    driver.drawText(
        text,
        x,
        y,
        color,
        font);
}

void DisplayManager::drawPixel(
    int16_t x,
    int16_t y,
    uint16_t color)
{
    driver.drawPixel(
        x,
        y,
        color);
}

void DisplayManager::drawLine(
    int16_t x0,
    int16_t y0,
    int16_t x1,
    int16_t y1,
    uint16_t color)
{
    driver.drawLine(
        x0,
        y0,
        x1,
        y1,
        color);
}

void DisplayManager::drawRect(
    int16_t x,
    int16_t y,
    int16_t w,
    int16_t h,
    uint16_t color)
{
    driver.drawRect(
        x,
        y,
        w,
        h,
        color);
}

void DisplayManager::fillRect(
    int16_t x,
    int16_t y,
    int16_t w,
    int16_t h,
    uint16_t color)
{
    driver.fillRect(
        x,
        y,
        w,
        h,
        color);
}

void DisplayManager::drawCircle(
    int16_t x,
    int16_t y,
    int16_t r,
    uint16_t color)
{
    driver.drawCircle(
        x,
        y,
        r,
        color);
}

void DisplayManager::fillCircle(
    int16_t x,
    int16_t y,
    int16_t r,
    uint16_t color)
{
    driver.fillCircle(
        x,
        y,
        r,
        color);
}

void DisplayManager::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
{
    driver.drawBitmap(x, y, bitmap, w, h, color);
}

void DisplayManager::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color)
{
    driver.fillRoundRect(x, y, w, h, radius, color);
}

void DisplayManager::setClipRect(int16_t x, int16_t y, int16_t w, int16_t h) { driver.setClipRect(x, y, w, h); }
void DisplayManager::clearClipRect() { driver.clearClipRect(); }