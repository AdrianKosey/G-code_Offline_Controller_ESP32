#pragma once

#include "../display/display_manager.h"
#include <vector>
#include "buzzer_manager.h"
struct BootItem {
    const char* name;
    bool success;
};

class SplashScreen
{
public:
    static void show(DisplayManager& display, BuzzerManager& buzzer, const std::vector<BootItem>& bootItems, unsigned long durationMs = 2500, std::function<void()> onFrame = nullptr);
};