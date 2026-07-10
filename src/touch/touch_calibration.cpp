#include "touch_calibration.h"


bool TouchCalibration::calibrate(
    IDisplayDriver& display,
    ITouchDriver& touch)
{

    display.clear();


    uint16_t points[5][2] =
    {
        {20,20},
        {300,20},
        {160,120},
        {20,220},
        {300,220}
    };


    for(int i = 0; i < 5; i++)
    {

        display.clear();


        drawCross(
            display,
            points[i][0],
            points[i][1]);


        uint16_t x;
        uint16_t y;


        while(true)
        {
            if(touch.getTouch(x,y))
            {
                calData[i*2] = x;
                calData[i*2+1] = y;

                break;
            }

            delay(10);
        }


        delay(300);
    }


    calibrated = true;


    return true;
}

void TouchCalibration::drawCross(
    IDisplayDriver& display,
    uint16_t x,
    uint16_t y)
{

    display.drawLine(
        x-10,
        y,
        x+10,
        y,
        0xFFFF);


    display.drawLine(
        x,
        y-10,
        x,
        y+10,
        0xFFFF);
}

void TouchCalibration::transform(
    uint16_t& x,
    uint16_t& y)
{

    x = map(
        x,
        calData[0],
        calData[2],
        0,
        320);


    y = map(
        y,
        calData[1],
        calData[3],
        0,
        240);

}

bool TouchCalibration::load()
{
    // Load calibration data from storage
    // For now, we will just return false to indicate no calibration data is available
    return false;
}

bool TouchCalibration::save()
{
    // Save calibration data to storage
    // For now, we will just return true to indicate success
    return true;
}

bool TouchCalibration::isCalibrated()
{
    return calibrated;
}