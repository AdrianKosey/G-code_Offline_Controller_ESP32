#pragma once

#include <TFT_eSPI.h>
#include "../../interface/idisplay_driver.h"
#include "../../../touch/interface/itouch_driver.h"
class ILI9341Driver : public IDisplayDriver, public ITouchDriver
{
public:
    ILI9341Driver();

    ~ILI9341Driver() override = default;

    bool begin() override;

    void clear(uint16_t color = TFT_BLACK) override;

    void setRotation(uint8_t rotation) override;

    uint16_t width() const override;

    uint16_t height() const override;

    void drawText(
        const String& text,
        int16_t x,
        int16_t y,
        uint16_t color = TFT_WHITE,
        uint8_t font = 2) override;

    void drawPixel(
        int16_t x,
        int16_t y,
        uint16_t color) override;

    void drawLine(
        int16_t x0,
        int16_t y0,
        int16_t x1,
        int16_t y1,
        uint16_t color) override;

    void drawRect(
        int16_t x,
        int16_t y,
        int16_t w,
        int16_t h,
        uint16_t color) override;

    void fillRect(
        int16_t x,
        int16_t y,
        int16_t w,
        int16_t h,
        uint16_t color) override;

    void drawCircle(
        int16_t x,
        int16_t y,
        int16_t r,
        uint16_t color) override;

    void fillCircle(
        int16_t x,
        int16_t y,
        int16_t r,
        uint16_t color) override;

    // Touch
    bool getTouch(
        uint16_t& x,
        uint16_t& y) override;

private:

    TFT_eSPI tft;
};