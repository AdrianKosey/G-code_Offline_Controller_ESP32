#pragma once
#include "touch_point.h"
#include <stdint.h>
struct TouchEvent
{
    bool pressed;

    bool released;

    bool clicked;

    bool longPressed;

    bool dragging;

    TouchPoint point;
};