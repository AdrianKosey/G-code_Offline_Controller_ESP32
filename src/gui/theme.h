#pragma once

#include <Arduino.h>

namespace Theme
{
    constexpr uint16_t Background = 0x11192b; 

    constexpr uint16_t Panel = 0x2124; // #2124

    constexpr uint16_t SidebarBackground = 0x182f4e; // #182f4e
    
    constexpr uint16_t SidebarBorderSelected = 0xecb86f; // #ecb86f

    constexpr uint16_t SidebarSelected = 0x334059; // #334059

    constexpr uint16_t Text = 0xFFFFFF; // #FFFFFF

    constexpr uint16_t TextSecondary = 0xAAAAAA; // #AAAAAA

    constexpr uint16_t Border = 0x000000; // #000000

    constexpr uint16_t Success = 0x00FF00; // #00FF00
    constexpr uint16_t Warning = 0xFFFF00; // #FFFF00
    constexpr uint16_t Error = 0xFF0000; // #FF0000

    constexpr uint16_t Progress = 0xecb86f; // #ecb86f
}