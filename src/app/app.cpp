#include "App.h"

App::App() : display(displayDriver), touch(displayDriver),
             button(
                 Rect{
                     20,
                     20,
                     120,
                     40},
                 "Inicio") {}

void App::begin()
{
    display.begin();

    constexpr bool FORCE_TOUCH_CALIBRATION = false; // Set to true to force touch calibration on every startup
    touch.begin(FORCE_TOUCH_CALIBRATION);

    display.clear(0x0000);
    button.draw(display);
}

void App::update()
{
    TouchEvent event = touch.poll();

    if (button.handleTouch(event))
    {
        button.setSelected(!button.isSelected());
    }

    button.draw(display);
}