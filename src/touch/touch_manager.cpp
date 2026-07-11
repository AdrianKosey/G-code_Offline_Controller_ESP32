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
    bool touched = driver.getTouch(x, y);

    TouchEvent event{};

    if (touched && !wasTouched)
    {
        event.type = TouchType::Pressed;
        event.point = { x, y };
    }
    else if (touched && wasTouched)
    {
        event.type = TouchType::Move;
        event.point = { x, y };
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

    if (touched)
        lastPoint = { x, y };

    wasTouched = touched;

    return event;
}