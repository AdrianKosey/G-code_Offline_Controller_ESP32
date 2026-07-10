#pragma once

#include "touch_event.h"
#include "touch_calibration.h"

#include "interface/itouch_driver.h"
class TouchManager
{
public:

    explicit TouchManager(ITouchDriver& driver);

    bool begin(bool forceCalibration = false);

    TouchEvent poll();

private:

    ITouchDriver& driver;

    TouchCalibration calibration;

    TouchEvent currentEvent;
};