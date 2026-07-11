#pragma once

#include "touch_event.h"
#include "touch_calibration.h"

#include "interface/itouch_driver.h"
class TouchManager
{
public:
    explicit TouchManager(ITouchDriver &driver);

    bool begin(bool forceCalibration = false);

    TouchEvent poll();

private:
    ITouchDriver &driver;
    TouchCalibration calibration;
    TouchEvent currentEvent;

    bool wasTouched = false;
    TouchPoint lastPoint{};

    static constexpr uint8_t RELEASE_DEBOUNCE_COUNT = 3;
    uint8_t releaseCandidateCount = 0;
};