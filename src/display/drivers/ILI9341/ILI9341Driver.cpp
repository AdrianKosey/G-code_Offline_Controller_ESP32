#include "ILI9341Driver.h"

ILI9341Driver::ILI9341Driver()
{
}

bool ILI9341Driver::begin()
{
    tft.init();

    tft.setRotation(1);

    tft.fillScreen(TFT_BLACK);

    return true;
}

void ILI9341Driver::clear(uint16_t color)
{
    tft.fillScreen(color);
}

void ILI9341Driver::setRotation(uint8_t rotation)
{
    tft.setRotation(rotation);
}

uint16_t ILI9341Driver::width() const
{
    return const_cast<ILI9341Driver*>(this)->tft.width();
}

uint16_t ILI9341Driver::height() const
{
    return const_cast<ILI9341Driver*>(this)->tft.height();
}

void ILI9341Driver::drawText(
    const String& text,
    int16_t x,
    int16_t y,
    uint16_t color,
    uint8_t font)
{
    tft.setTextColor(color);

    tft.setTextFont(font);

    tft.drawString(text, x, y);
}

void ILI9341Driver::drawPixel(
    int16_t x,
    int16_t y,
    uint16_t color)
{
    tft.drawPixel(x, y, color);
}

void ILI9341Driver::drawLine(
    int16_t x0,
    int16_t y0,
    int16_t x1,
    int16_t y1,
    uint16_t color)
{
    tft.drawLine(x0, y0, x1, y1, color);
}

void ILI9341Driver::drawRect(
    int16_t x,
    int16_t y,
    int16_t w,
    int16_t h,
    uint16_t color)
{
    tft.drawRect(x, y, w, h, color);
}

void ILI9341Driver::fillRect(
    int16_t x,
    int16_t y,
    int16_t w,
    int16_t h,
    uint16_t color)
{
    tft.fillRect(x, y, w, h, color);
}

void ILI9341Driver::drawCircle(
    int16_t x,
    int16_t y,
    int16_t r,
    uint16_t color)
{
    tft.drawCircle(x, y, r, color);
}

void ILI9341Driver::fillCircle(
    int16_t x,
    int16_t y,
    int16_t r,
    uint16_t color)
{
    tft.fillCircle(x, y, r, color);
}

bool ILI9341Driver::getTouch(
    uint16_t& x,
    uint16_t& y)
{
    return tft.getTouch(&x, &y);
}