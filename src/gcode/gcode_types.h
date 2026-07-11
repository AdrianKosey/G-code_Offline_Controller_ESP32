#pragma once

enum class GCodeMotionType
{
    None,
    Rapid,   // G0
    Linear,  // G1
    ArcCW,   // G2
    ArcCCW   // G3
};

enum class SpindleState
{
    Off,
    ClockwiseOn,
    CounterClockwiseOn
};

struct GCodePosition
{
    float x = 0;
    float y = 0;
    float z = 0;
};

struct GCodeCommand
{
    GCodeMotionType motion = GCodeMotionType::None;

    GCodePosition target;
    bool hasTarget = false; 

    float feedRate = 0;
    float spindleSpeed = 0;
    SpindleState spindle = SpindleState::Off;

    bool isProgramEnd = false; // M2 / M30
};