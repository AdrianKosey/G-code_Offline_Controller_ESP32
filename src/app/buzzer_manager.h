#pragma once

#include <Arduino.h>
#include "../../include/config.h"

class BuzzerManager
{
public:
    void begin();
    void update(); 

    void beep();

    void setEnabled(bool enabled);
    bool isEnabled() const;
    void playCompletionMelody();
private:
    bool enabled = BUZZER_ENABLED_DEFAULT;
    bool active = false;
    unsigned long stopAt = 0;

    struct MelodyNote { uint32_t frequency; uint16_t durationMs; };

    static constexpr uint8_t MELODY_LENGTH = 4;
    MelodyNote melody[MELODY_LENGTH] = {
        { 1500, 100 },
        { 1800, 100 },
        { 2200, 100 },
        { 2800, 200 }
    };

    bool playingMelody = false;
    uint8_t melodyIndex = 0;
    unsigned long melodyNoteEndsAt = 0;
};