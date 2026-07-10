#pragma once
#include <Arduino.h>
#include "interface/idisplay_driver.h"

class DisplayManager
{
public:

    explicit DisplayManager(IDisplayDriver& driver);

    bool begin();

    void update();

    void clear(uint16_t color = 0x0000); // Default color is black

    void setRotation(uint8_t rotation);

    uint16_t width() const;

    uint16_t height() const;

    void drawText(
        const String& text,
        int16_t x,
        int16_t y,
        uint16_t color = 0xFFFF, // Default color is white
        uint8_t font = 2);

    void drawPixel(
        int16_t x,
        int16_t y,
        uint16_t color);

    void drawLine(
        int16_t x0,
        int16_t y0,
        int16_t x1,
        int16_t y1,
        uint16_t color);

    void drawRect(
        int16_t x,
        int16_t y,
        int16_t w,
        int16_t h,
        uint16_t color);

    void fillRect(
        int16_t x,
        int16_t y,
        int16_t w,
        int16_t h,
        uint16_t color);

    void drawCircle(
        int16_t x,
        int16_t y,
        int16_t r,
        uint16_t color);

    void fillCircle(
        int16_t x,
        int16_t y,
        int16_t r,
        uint16_t color);

private:

    IDisplayDriver& driver;
};