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
    if (playingMelody)
    {
        if (millis() >= melodyNoteEndsAt)
        {
            if (melodyIndex >= MELODY_LENGTH)
            {
                ledcWriteTone(BUZZER_PIN, 0);
                playingMelody = false;
            }
            else
            {
                ledcWriteTone(BUZZER_PIN, melody[melodyIndex].frequency);
                melodyNoteEndsAt = millis() + melody[melodyIndex].durationMs;
                melodyIndex++;
            }
        }

        return;
    }

    if (active && millis() >= stopAt)
    {
        ledcWriteTone(BUZZER_PIN, 0);
        active = false;
    }
}

void BuzzerManager::playCompletionMelody()
{
    if (!enabled || BUZZER_PIN < 0)
        return;

    playingMelody = true;
    melodyIndex = 0;
    melodyNoteEndsAt = 0;
}