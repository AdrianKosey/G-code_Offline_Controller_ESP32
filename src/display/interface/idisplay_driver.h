#pragma once

#include <Arduino.h>
class IDisplayDriver
{
public:

    virtual ~IDisplayDriver() = default;

    virtual bool begin() = 0;

    virtual void clear(uint16_t color = 0x0000) = 0; // Default color is black

    virtual void setRotation(uint8_t rotation) = 0;

    virtual uint16_t width() const = 0;

    virtual uint16_t height() const = 0;

    virtual void drawText(
        const String& text,
        int16_t x,
        int16_t y,
        uint16_t color = 0xFFFF, // Default color is white
        uint8_t font = 2) = 0;

    virtual void drawPixel(
        int16_t x,
        int16_t y,
        uint16_t color) = 0;

    virtual void drawLine(
        int16_t x0,
        int16_t y0,
        int16_t x1,
        int16_t y1,
        uint16_t color) = 0;

    virtual void drawRect(
        int16_t x,
        int16_t y,
        int16_t w,
        int16_t h,
        uint16_t color) = 0;

    virtual void fillRect(
        int16_t x,
        int16_t y,
        int16_t w,
        int16_t h,
        uint16_t color) = 0;

    virtual void drawCircle(
        int16_t x,
        int16_t y,
        int16_t r,
        uint16_t color) = 0;

    virtual void fillCircle(
        int16_t x,
        int16_t y,
        int16_t r,
        uint16_t color) = 0;
};