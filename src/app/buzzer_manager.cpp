#include "buzzer_manager.h"
#include "../../include/pins.h"

void BuzzerManager::begin()
{
    if (BUZZER_PIN >= 0)
        ledcAttach(BUZZER_PIN, BUZZER_FREQUENCY, 8);
}

void BuzzerManager::setEnabled(bool value) { enabled = value; }
bool BuzzerManager::isEnabled() const { return enabled; }

void BuzzerManager::beep()
{
    if (!enabled || BUZZER_PIN < 0)
        return;

    ledcWriteTone(BUZZER_PIN, BUZZER_FREQUENCY);
    active = true;
    stopAt = millis() + BUZZER_DURATION_MS;
}

void BuzzerManager::update()
{
    if (active && millis() >= stopAt)
    {
        ledcWriteTone(BUZZER_PIN, 0);
        active = false;
    }
}