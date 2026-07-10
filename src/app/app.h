#pragma once
#include "../display/display_manager.h"
#include "../touch/touch_manager.h"
#include "../display/drivers/ILI9341/ILI9341Driver.h"
class App
{
public:
    App();
    void begin();
    void update();

private:
    ILI9341Driver displayDriver;
    DisplayManager display;
    TouchManager touch;
};