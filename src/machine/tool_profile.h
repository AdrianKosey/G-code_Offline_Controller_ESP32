#pragma once
#include <Arduino.h>

enum class ToolType { Spindle, Laser };

struct ToolProfile
{
    String name;
    ToolType type = ToolType::Spindle;
    float diameterMm = 3.0f;
    float defaultPower = 10.0f;
    uint32_t usageSeconds = 0;
};