#include "touch_calibration.h"

bool TouchCalibration::calibrate(ITouchDriver& touch)
{

    if (!touch.calibrateTouch(calData))
        return false;

    calibrated = true;

    save();

    return true;
}

bool TouchCalibration::save()
{
    preferences.begin("touch", false);

    preferences.putBytes(
        "cal",
        calData,
        sizeof(calData));

    preferences.putBool(
        "valid",
        true);

    preferences.end();

    return true;
}

bool TouchCalibration::load(ITouchDriver& touch)
{
    preferences.begin("touch", true);

    bool valid =
        preferences.getBool("valid", false);

    if (!valid)
    {
        preferences.end();
        return false;
    }

    preferences.getBytes(
        "cal",
        calData,
        sizeof(calData));

    preferences.end();

    touch.setTouchCalibration(calData);

    calibrated = true;

    return true;
}

bool TouchCalibration::isCalibrated() const
{
    return calibrated;
}
