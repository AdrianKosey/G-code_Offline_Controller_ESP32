#pragma once

#include <Arduino.h>

struct Rect
{
    int16_t x;
    int16_t y;

    int16_t width;
    int16_t height;

    bool contains(uint16_t px, uint16_t py) const
    {
        return
            px >= x &&
            px < x + width &&
            py >= y &&
            py < y + height;
    }
};