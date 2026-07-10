#include "touch_manager.h"

TouchManager::TouchManager(IDisplayDriver& display, ITouchDriver& driver)
    : display(display), driver(driver)
{
}

bool TouchManager::begin()
{
    if (!calibration.load())
    {
        calibration.calibrate(display, driver);

        calibration.save();
    }

    return true;
}

void TouchManager::update()
{
    currentEvent = {};

    uint16_t x;
    uint16_t y;

    if (driver.getTouch(x, y))
    {
        calibration.transform(x, y);

        currentEvent.pressed = true;

        currentEvent.point.x = x;

        currentEvent.point.y = y;
    }
}

TouchEvent TouchManager::poll() const
{
    return currentEvent;
}