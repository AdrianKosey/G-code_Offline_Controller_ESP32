#pragma once

#include <Arduino.h>
#include "../../display/display_manager.h"
#include "../../touch/touch_event.h"
#include "../../app/app_settings_manager.h"

class ScreenSleepManager
{
public:
    ScreenSleepManager(DisplayManager& display, AppSettingsManager& appSettings);

    bool handleTouch(const TouchEvent& event);

    void update();

    bool isSleeping() const;

private:
    enum class State { Awake, Dimmed, Sleeping };
    DisplayManager& display;
    AppSettingsManager& appSettings;

    State state = State::Awake;
    static constexpr unsigned long DIM_BEFORE_SLEEP_MS = 5000; 
    static constexpr uint8_t DIM_BRIGHTNESS_PERCENT = 10;
    bool sleeping = false;
    unsigned long lastActivityAt = 0;

    void sleep();
    void wake();
};