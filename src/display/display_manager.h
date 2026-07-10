#pragma once
#include <Arduino.h>
#include "interface/idisplay_driver.h"

class DisplayManager
{
public:
    DisplayManager(IDisplayDriver& driver);
    bool begin();
    void update();
    void drawText(const String& text, int16_t x, int16_t y, uint16_t color, uint8_t font);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void clear(uint16_t color);
    void setRotation(uint8_t rotation);
    
private:
    IDisplayDriver& driver;
};