#pragma once

#include "../display/display_manager.h"

class SplashScreen
{
public:
    static void show(DisplayManager& display, unsigned long durationMs = 5000);
};