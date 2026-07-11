#include "touch_manager.h"

TouchManager::TouchManager(ITouchDriver& driver) : driver(driver){}

bool TouchManager::begin(bool forceCalibration)
{
    if (forceCalibration || !calibration.load(driver))
    {
        if (!calibration.calibrate(driver))
            return false;
    }
    return true;
}


TouchEvent TouchManager::poll()
{
    uint16_t x, y;
    bool rawTouched = driver.getTouch(x, y);

    bool touched;

    if (rawTouched)
    {
        releaseCandidateCount = 0;
        touched = true;
    }
    else if (wasTouched)
    {
        releaseCandidateCount++;
        touched = releaseCandidateCount < RELEASE_DEBOUNCE_COUNT;
    }
    else
    {
        touched = false;
    }

    TouchEvent event{};

    if (touched && !wasTouched)
    {
        event.type = TouchType::Pressed;
        event.point = { x, y };
    }
    else if (touched && wasTouched)
    {
        event.type = TouchType::Move;
        event.point = rawTouched ? TouchPoint{ x, y } : lastPoint;
    }
    else if (!touched && wasTouched)
    {
        event.type = TouchType::Released;
        event.point = lastPoint;
    }
    else
    {
        event.type = TouchType::None;
    }

    if (rawTouched)
        lastPoint = { x, y };

    wasTouched = touched;

    return event;
}