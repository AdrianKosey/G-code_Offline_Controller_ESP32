#pragma once
#include <Arduino.h>

inline String formatElapsed(unsigned long totalSeconds)
{
    unsigned long h = totalSeconds / 3600;
    unsigned long m = (totalSeconds % 3600) / 60;
    unsigned long s = totalSeconds % 60;

    char buf[16];
    snprintf(buf, sizeof(buf), "%02lu:%02lu:%02lu", h, m, s);
    return String(buf);
}