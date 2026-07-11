#pragma once
#include <Arduino.h>
// Screen (TFT_eSPI) - configured in User_Setup, reference only here
// TFT_MISO 19, TFT_MOSI 23, TFT_SCLK 18, TFT_CS 15, TFT_DC 2, TFT_RST 4, TOUCH_CS 5

// microSD card - shares SPI bus with the screen, has its own CS
constexpr uint8_t SD_CS_PIN = 21;