#pragma once

#include <Arduino.h>

namespace Theme
{
    constexpr uint16_t RGB565(uint32_t rgb888)
    {
        uint8_t r = (rgb888 >> 16) & 0xFF;
        uint8_t g = (rgb888 >> 8) & 0xFF;
        uint8_t b = rgb888 & 0xFF;

        return
            ((r & 0xF8) << 8) |
            ((g & 0xFC) << 3) |
            (b >> 3);
    }

    constexpr uint16_t Background = RGB565(0x11192b);

    constexpr uint16_t Panel = RGB565(0x212437);

    constexpr uint16_t SidebarBackground = RGB565(0x182f4e);

    constexpr uint16_t SidebarBorderSelected = RGB565(0xecb86f);

    constexpr uint16_t SidebarSelected = RGB565(0x334059);

    constexpr uint16_t Text = RGB565(0xFFFFFF);

    constexpr uint16_t TextSecondary = RGB565(0xAAAAAA);

    constexpr uint16_t Border = RGB565(0x000000);

    constexpr uint16_t Success = RGB565(0x00FF00);
    constexpr uint16_t Warning = RGB565(0xFFFF00);
    constexpr uint16_t Error = RGB565(0xFF0000);

    constexpr uint16_t Progress = RGB565(0xecb86f);
}