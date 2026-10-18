#include "splash_screen.h"
#include "../gui/theme.h"
#include "../gui/icons.h"

void SplashScreen::show(DisplayManager& display, BuzzerManager& buzzer, const std::vector<BootItem>& bootItems, unsigned long durationMs, std::function<void()> onFrame)
{
    int16_t screenW = display.width();
    int16_t screenH = display.height();
    int16_t cx = screenW / 2;
    int16_t cy = screenH / 2;

    // ==========================================
    // Effect of boot logs with a progress bar
    // ==========================================
    display.clear(0x0000);

    size_t totalLogs = bootItems.size();
    if (totalLogs == 0) totalLogs = 1;

    unsigned long bootTimeAlloc = durationMs * 0.60f; // 60% of time
    unsigned long timePerLog = bootTimeAlloc / totalLogs;

    int16_t logY = 15;
    for (size_t i = 0; i < bootItems.size(); i++) {
        unsigned long t0 = millis();
        while (millis() - t0 < timePerLog) {
            if (onFrame) onFrame();
            buzzer.update();
            delay(10);
        }

        if (bootItems[i].success) {
            display.drawText("[  OK  ] ", 10, logY, 0x07E0, 1);
            buzzer.beep();
        } else {
            display.drawText("[ FAIL ] ", 10, logY, 0xF800, 1);
            buzzer.beep();
        }
        
        display.drawText(bootItems[i].name, 70, logY, 0xFFFF, 1);
        logY += 14;
    }

    unsigned long pauseStart = millis();
    while (millis() - pauseStart < 2000) {
        if (onFrame) onFrame();
        buzzer.update();
        delay(10);
    }

    // ==========================================
    // Logo Splash Screen
    // ==========================================
    display.clear(0x0000);

    int16_t logoW = Icons::SPLASH_LOGO_WIDTH;   // 200
    int16_t logoH = Icons::SPLASH_LOGO_HEIGHT;  // 200
    int16_t logoX = cx - (logoW / 2);
    int16_t logoY = cy - (logoH / 2);

    display.drawBitmap(logoX, logoY, Icons::LogoKosey, logoW, logoH, 0xFFFF);

    unsigned long logoStartTime = millis();
    unsigned long logoDuration = durationMs - bootTimeAlloc; 

    while (millis() - logoStartTime < logoDuration) {
        buzzer.update();
        delay(30);
        if (onFrame) onFrame();
    }
}