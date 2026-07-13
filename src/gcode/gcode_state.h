#pragma once
#include "gcode_types.h"


struct GCodeState
{
    GCodePosition position;

    bool absoluteMode = true; // G90 (true) / G91 (false)
    bool metric = true;       // G21 (true, mm) / G20 (false, inchs)

    float feedRate = 0;
    float spindleSpeed = 0;
    SpindleState spindle = SpindleState::Off;

    GCodeMotionType lastMotion = GCodeMotionType::Rapid; // G1 without explicit "G" repeats the last modal

    uint8_t plane = 17;        // G17=XY, G18=XZ, G19=YZ
    uint8_t coordSystem = 54;  // G54-G59
    uint8_t toolNumber = 0;    // Txx
};