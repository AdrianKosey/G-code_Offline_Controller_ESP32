#include "App.h"

static constexpr uint8_t SIDEBAR_WIDTH = 60;

static const SidebarItem sidebarItems[] = {
    {IconId::Home, "Inicio"},
    {IconId::Files, "Archivos"},
    {IconId::Prepare, "Preparar"},
    {IconId::Tools, "Tools"},
    {IconId::Settings, "Configrar"},
};

App::App()
    : display(displayDriver),
      touch(displayDriver),
      screenManager(display, Rect{0, 0, SIDEBAR_WIDTH, 240}, sidebarItems, 5)
{
}

void App::begin()
{

    SPI.begin(SCK, MISO, MOSI, SD_CS_PIN);
    sdReady = SD.begin(SD_CS_PIN, SPI, 4000000);
    if (!sdReady)
    {
        Serial.println("SD: card not detected or initialization failed");
    }

    display.begin();

    constexpr bool FORCE_TOUCH_CALIBRATION = false;
    touch.begin(FORCE_TOUCH_CALIBRATION);

    display.clear(Theme::Background);

    screenManager.registerScreen(0, &homeScreen);
    screenManager.registerScreen(1, &filesScreen);
    screenManager.registerScreen(2, &prepareScreen);
    screenManager.registerScreen(3, &toolsScreen);
    screenManager.registerScreen(4, &settingsScreen);

    screenManager.draw();
}

void App::update()
{
    TouchEvent event = touch.poll();

    screenManager.handleTouch(event);
    screenManager.update();
    screenManager.draw();
}