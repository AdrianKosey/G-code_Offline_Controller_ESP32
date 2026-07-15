#pragma once
#include <Arduino.h>
// Screen (TFT_eSPI) - configured in User_Setup, reference only here
// TFT_MISO 19, TFT_MOSI 23, TFT_SCLK 18, TFT_CS 15, TFT_DC 2, TFT_RST 4, TOUCH_CS 5

// microSD card - shares SPI bus with the screen, has its own CS
constexpr uint8_t SD_CS_PIN = 21;

constexpr uint8_t GRBL_RX_PIN = 16;
constexpr uint8_t GRBL_TX_PIN = 17;

constexpr int8_t TFT_BACKLIGHT_PIN = 22;

constexpr int8_t BUZZER_PIN = 13;