#include "splash_screen.h"
#include "../gui/theme.h"
#include "../../include/config.h"
#include <math.h>

float easeInOutCubic(float t) {
    return t < 0.5f ? 4.0f * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
}

void SplashScreen::show(DisplayManager& display, unsigned long durationMs)
{
    display.clear(Theme::Background);

    int16_t screenW = display.width();
    int16_t screenH = display.height();
    int16_t cx = screenW / 2;
    int16_t cy = screenH / 2 - 30;
    int16_t radius = 35;

    int16_t nameWidth = display.getTextWidth(CONTROL_NAME, 2);
    display.drawText(CONTROL_NAME, cx - (nameWidth / 2), cy + 60, Theme::Text, 2);
    display.drawText(CONTROL_VERSION, cx - (nameWidth / 2), cy + 80, Theme::Text, 1);
    display.drawText("by Kosey", cx - (nameWidth / 2), cy + 105, Theme::Text, 1);

    unsigned long startTime = millis();

    while (millis() - startTime < durationMs)
    {
        unsigned long elapsed = millis() - startTime;
        float globalProgress = (float)elapsed / durationMs; // 0.0 a 1.0

        float spinProgress = fmod(globalProgress * 3.0f, 1.0f); 
        float easedSpin = easeInOutCubic(spinProgress);

        float arcLength = 45.0f + (sin(globalProgress * M_PI * 6.0f) + 1.0f) * 60.0f; 
        
        float startAngle = easedSpin * 360.0f;
        float endAngle = startAngle + arcLength;

        display.fillRect(cx - radius - 4, cy - radius - 4, (radius * 2) + 8, (radius * 2) + 8, Theme::Background);

        for (float a = startAngle; a <= endAngle; a += 3.0f) {
            float rad = a * M_PI / 180.0f;
            int16_t px = cx + cos(rad) * radius;
            int16_t py = cy + sin(rad) * radius;
            
            display.fillCircle(px, py, 3, Theme::Progress);
        }

        int16_t barWidth = 160;
        int16_t barX = cx - (barWidth / 2);
        int16_t barY = cy + 95;

        display.drawRect(barX, barY, barWidth, 6, Theme::SidebarBackground);

        int16_t currentFill = (int16_t)(easeInOutCubic(globalProgress) * (barWidth - 2));
        if (currentFill > 0) {
            display.fillRect(barX + 1, barY + 1, currentFill, 4, Theme::Progress);
        }

        delay(15); 
    }

    display.clear(Theme::Background);
}