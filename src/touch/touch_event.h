#pragma once
#include "touch_point.h"
#include <stdint.h>

enum class TouchType
{
    None,
    Pressed,
    Released,
    Move,
    longPress
};
struct TouchEvent
{
    TouchType type;
    TouchPoint point;
};