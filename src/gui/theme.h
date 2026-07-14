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

    constexpr uint16_t Background = RGB565(0x11192b); // #11192b

    constexpr uint16_t Panel = RGB565(0x182f4e); // #182f4e

    constexpr uint16_t JobPanel = RGB565(0x3f6b8e); // #3f6b8e

    constexpr uint16_t SidebarBackground = RGB565(0x182f4e); // #182f4e

    constexpr uint16_t SidebarBorderSelected = RGB565(0xecb86f); // #ecb86f

    constexpr uint16_t SidebarSelected = RGB565(0x334059); // #334059

    constexpr uint16_t Text = RGB565(0xFFFFFF); // #FFFFFF

    constexpr uint16_t TextBlack = RGB565(0x00000); // #00000

    constexpr uint16_t TextSecondary = RGB565(0xAAAAAA); // #AAAAAA

    constexpr uint16_t Border = RGB565(0x000000); // #000000

    constexpr uint16_t Success = RGB565(0x00FF00); // #00FF00
    constexpr uint16_t Warning = RGB565(0xFFFF00); // #FFFF00
    constexpr uint16_t Error = RGB565(0xFF0000); // #FF0000

    constexpr uint16_t ButtonPlayBackground = RGB565(0x5288ad); // #5288ad
    constexpr uint16_t ButtonPauseBackground = RGB565(0x5288ad); // #5288ad
    constexpr uint16_t ButtonStopBackground = RGB565(0xFF0000); // #FF0000

    constexpr uint16_t Progress = RGB565(0xecb86f); // #ECB86F
    constexpr uint16_t Progress2 = RGB565(0x7f00e7); // #7f00e7
    constexpr uint16_t Progress3 = RGB565(0x6fc4ec); // #6fc4ec
}