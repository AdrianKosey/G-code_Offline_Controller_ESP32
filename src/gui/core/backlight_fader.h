#pragma once
#include <Arduino.h>
#include "../../display/display_manager.h"

class BacklightFader
{
public:
    BacklightFader(DisplayManager& display) : display(display) {}

    void fadeTo(uint8_t targetPercent, unsigned long durationMs)
    {
        startLevel = current;
        target = targetPercent;
        duration = durationMs;
        startAt = millis();
        active = true;
    }

    void update()
    {
        if (!active) return;

        unsigned long t = millis() - startAt;

        if (t >= duration)
        {
            current = target;
            active = false;
        }
        else
        {
            current = startLevel + ((int)target - (int)startLevel) * (long)t / (long)duration;
        }

        display.setBacklightLevel(current);
    }

private:
    DisplayManager& display;
    int current = 0, startLevel = 0;
    uint8_t target = 0;
    unsigned long startAt = 0, duration = 0;
    bool active = false;
};