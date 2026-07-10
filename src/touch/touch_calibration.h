#pragma once

#include <Arduino.h>

#include "../display/interface/idisplay_driver.h"
#include "interface/itouch_driver.h"

class TouchCalibration
{
public:
    bool load();

    bool save();

    bool isCalibrated();

    bool calibrate(
        IDisplayDriver &display,
        ITouchDriver &touch);

    void transform(
        uint16_t &x,
        uint16_t &y);

    void drawCross(
        IDisplayDriver &display,
        uint16_t x,
        uint16_t y);

private:

    bool calibrated = false;

    uint16_t calData[5];
};