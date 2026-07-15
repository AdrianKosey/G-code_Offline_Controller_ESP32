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

private:
    bool enabled = BUZZER_ENABLED_DEFAULT;
    bool active = false;
    unsigned long stopAt = 0;
};