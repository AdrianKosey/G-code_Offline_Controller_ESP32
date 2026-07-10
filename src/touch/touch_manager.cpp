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
    currentEvent = {};

    uint16_t x;
    uint16_t y;

    if (driver.getTouch(x, y))
    {
        currentEvent.type = TouchType::Pressed;
        currentEvent.point.x = x;
        currentEvent.point.y = y;
    }

    return currentEvent;
}