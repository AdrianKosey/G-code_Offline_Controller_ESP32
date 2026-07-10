#include "App.h"

App::App() : display(displayDriver), touch(displayDriver) {}

void App::begin()
{   
    display.begin();

    constexpr bool FORCE_TOUCH_CALIBRATION = false; // Set to true to force touch calibration on every startup
    touch.begin(FORCE_TOUCH_CALIBRATION);

    display.clear(0x0000FF); // Clear display with blue color

}

void App::update()
{
    TouchEvent event = touch.poll();

    if (event.pressed)
    {
        display.clear(0x0000FF); // Clear display with blue color
        display.drawText(
            String(event.point.x),
            10,
            10);

        display.drawText(
            String(event.point.y),
            10,
            35);
        Serial.print("Touch at: ");
        Serial.print(event.point.x);
        Serial.print(", ");
        Serial.println(event.point.y);
    }
}