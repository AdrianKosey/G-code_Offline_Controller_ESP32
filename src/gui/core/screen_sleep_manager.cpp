#include "screen_sleep_manager.h"

ScreenSleepManager::ScreenSleepManager(DisplayManager& display, AppSettingsManager& appSettings)
    : display(display), appSettings(appSettings)
{
    lastActivityAt = millis();
}

bool ScreenSleepManager::isSleeping() const { return state == State::Sleeping; }

void ScreenSleepManager::wake()
{
    state = State::Awake;
    lastActivityAt = millis();

    if (display.hasBacklightControl())
        display.setBacklightLevel(100);
}

bool ScreenSleepManager::handleTouch(const TouchEvent& event)
{
    if (event.type == TouchType::None)
        return false;

    if (state == State::Sleeping || state == State::Dimmed)
    {
        if (event.type == TouchType::Pressed)
        {
            wake();
            return true; // consumed - should not activate any UI buttons
        }

        return (state == State::Sleeping); // If it's completely asleep, ignore everything else; if it's only dimmed, let it pass
    }

    lastActivityAt = millis();
    return false;
}

void ScreenSleepManager::update()
{
    if (!appSettings.isScreenSleepEnabled())
    {
        if (state != State::Awake)
            wake(); // If the option was disabled while it was asleep/dimmed, it will wake up

        return;
    }

    unsigned long timeoutMs = (unsigned long)appSettings.getScreenSleepMinutes() * 60000UL;
    unsigned long elapsed = millis() - lastActivityAt;

    if (state == State::Awake && elapsed > (timeoutMs > DIM_BEFORE_SLEEP_MS ? timeoutMs - DIM_BEFORE_SLEEP_MS : 0))
    {
        state = State::Dimmed;

        if (display.hasBacklightControl())
            display.setBacklightLevel(DIM_BRIGHTNESS_PERCENT);
    }
    else if (state == State::Dimmed && elapsed > timeoutMs)
    {
        state = State::Sleeping;

        if (display.hasBacklightControl())
            display.setBacklightLevel(0);
        else
            display.fillRect(0, 0, display.width(), display.height(), 0x0000);
    }
}