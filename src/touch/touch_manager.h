#pragma once

#include "touch_event.h"
#include "touch_calibration.h"

#include "interface/itouch_driver.h"
#include "../display/interface/idisplay_driver.h"
class TouchManager
{
public:

    explicit TouchManager(IDisplayDriver& display, ITouchDriver& driver);

    bool begin();

    void update();

    TouchEvent poll() const;

private:

    ITouchDriver& driver;
    IDisplayDriver& display;

    TouchCalibration calibration;

    TouchEvent currentEvent;
};