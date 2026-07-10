#pragma once

#include <Arduino.h>
#include <Preferences.h>

#include "../display/interface/idisplay_driver.h"
#include "interface/itouch_driver.h"

class TouchCalibration
{
public:
    bool load(ITouchDriver &touch);
    bool save();
    bool isCalibrated() const;
    bool calibrate(ITouchDriver &touch);

private:
    Preferences preferences;
    bool calibrated = false;
    uint16_t calData[5];
};