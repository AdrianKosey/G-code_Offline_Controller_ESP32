#pragma once
#include <Arduino.h>
class ITouchDriver
{
public:

    virtual ~ITouchDriver() = default;

    virtual bool begin() = 0;

    virtual bool getTouch(
        uint16_t& x,
        uint16_t& y) = 0;
};