#pragma once
#include "gcode_types.h"


struct GCodeState
{
    GCodePosition position;

    bool absoluteMode = true; // G90 (true) / G91 (false)
    bool metric = true;       // G21 (true, mm) / G20 (false, pulgadas)

    float feedRate = 0;
    SpindleState spindle = SpindleState::Off;

    GCodeMotionType lastMotion = GCodeMotionType::Rapid; // G1 without explicit "G" repeats the last modal
};