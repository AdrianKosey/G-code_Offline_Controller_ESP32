#include "App.h"

App::App() : display(displayDriver), touch(displayDriver, displayDriver) {}

void App::begin()
{
    display.begin();

    touch.begin();

    display.clear(1023);

}

void App::update()
{
    touch.update();
    
}