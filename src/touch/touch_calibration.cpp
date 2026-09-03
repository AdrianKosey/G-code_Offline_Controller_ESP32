#include "touch_calibration.h"

namespace
{
    // Increase this whenever the physical display orientation or its rotation
    // changes.  Calibration data is expressed in display coordinates, so data
    // saved for the former rotation must not be reused.
    constexpr uint8_t CALIBRATION_REVISION = 2;
}

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

    preferences.putUChar(
        "revision",
        CALIBRATION_REVISION);

    preferences.end();

    return true;
}

bool TouchCalibration::load(ITouchDriver& touch)
{
    preferences.begin("touch", true);

    bool valid =
        preferences.getBool("valid", false);

    uint8_t revision = preferences.getUChar("revision", 0);

    if (!valid || revision != CALIBRATION_REVISION)
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
