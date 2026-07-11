#include "App.h"

static constexpr uint8_t SIDEBAR_WIDTH = 60;
static constexpr int16_t HEADER_HEIGHT = 30;
static constexpr int16_t SCREEN_HEIGHT = 240;

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
      screenManager(
          Rect{0, 0, SIDEBAR_WIDTH, SCREEN_HEIGHT},
          sidebarItems,
          5,
          Rect{SIDEBAR_WIDTH, 0, 320 - SIDEBAR_WIDTH, HEADER_HEIGHT}),
          confirmModal(Rect{0, 0, 320, 240}),
        loadingModal(Rect{0, 0, 320, 240})
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

    screenManager.registerScreen(0, &homeScreen, "Home");
    screenManager.registerScreen(1, &filesScreen, "Files");
    screenManager.registerScreen(2, &prepareScreen, "Prepare");
    screenManager.registerScreen(3, &toolsScreen, "Tools");
    screenManager.registerScreen(4, &settingsScreen, "Settings");

    filesScreen.setOnFileSelected([this](const String& path) {
        pendingFilePath = path;

        int lastSlash = path.lastIndexOf('/');
        String filename = (lastSlash >= 0) ? path.substring(lastSlash + 1) : path;

        confirmModal.show("Cargar " + filename + "?");
    });

    confirmModal.setOnConfirm([this]() {
        loadingModal.show("Cargando...");
        loadingModal.draw(display);

        homeScreen.loadJob(pendingFilePath);

        loadingModal.hide();
        screenManager.switchToScreen(0);
        screenManager.invalidateAll();
    });

    confirmModal.setOnCancel([this]() {
        screenManager.invalidateAll();
    });

    screenManager.showInitialScreen(0);
    screenManager.setSdStatus(sdReady);
    screenManager.setWifiStatus(false);

    screenManager.draw(display);
}

void App::update()
{
    TouchEvent event = touch.poll();

    if (confirmModal.isVisible())
        confirmModal.handleTouch(event);
    else if (loadingModal.isVisible())
        loadingModal.handleTouch(event);
    else
    {
        screenManager.handleTouch(event);
        screenManager.update();
    }

    screenManager.draw(display);
    confirmModal.draw(display);
    loadingModal.draw(display);
}