#include "App.h"

static constexpr uint8_t SIDEBAR_WIDTH = 60;
static constexpr int16_t HEADER_HEIGHT = 30;
static constexpr int16_t SCREEN_HEIGHT = 240;

static SidebarItem sidebarItemsBuffer[5]; 

static const SidebarItem* buildSidebarItems()
{
    sidebarItemsBuffer[0] = { IconId::Home, tr(StringId::Sidebar_Home) };
    sidebarItemsBuffer[1] = { IconId::Files, tr(StringId::Sidebar_Files) };
    sidebarItemsBuffer[2] = { IconId::Prepare, tr(StringId::Sidebar_Jog) };
    sidebarItemsBuffer[3] = { IconId::Tools, tr(StringId::Sidebar_Tools) };
    sidebarItemsBuffer[4] = { IconId::Settings, tr(StringId::Sidebar_Settings) };

    return sidebarItemsBuffer;
}
App::App()
    : display(displayDriver),
      touch(displayDriver),
      screenManager(
          Rect{0, 0, SIDEBAR_WIDTH, SCREEN_HEIGHT},
          buildSidebarItems(),
          5,
          Rect{SIDEBAR_WIDTH, 0, 320 - SIDEBAR_WIDTH, HEADER_HEIGHT}),
      confirmModal(Rect{0, 0, 320, 240}),
      loadingModal(Rect{0, 0, 320, 240}),
      jobRunner(grblController),
      homeScreen(grblController),
      jogScreen(grblController, appSettings),
      toolsScreen(grblController),
      framingRunner(grblController),
      settingsScreen(wifiManager, grblController, appSettings)
{
    g_appSettings = &appSettings;
}

void App::begin()
{
    // grblController.begin(Serial2, 115200, GRBL_RX_PIN, GRBL_TX_PIN);
    grblController.beginSimulated();
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

    screenManager.registerScreen(0, &homeScreen, tr(StringId::Sidebar_Home));
    screenManager.registerScreen(1, &filesScreen, tr(StringId::Sidebar_Files));
    screenManager.registerScreen(2, &jogScreen, tr(StringId::Sidebar_Jog));
    screenManager.registerScreen(3, &toolsScreen, tr(StringId::Sidebar_Tools));
    screenManager.registerScreen(4, &settingsScreen, tr(StringId::Sidebar_Settings));

    wifiManager.begin();

    homeScreen.setOnPlayPause([this]()
                              {
        JobState state = jobRunner.getState();

        if (state == JobState::Running)
            jobRunner.pause();
        else if (state == JobState::Paused)
            jobRunner.resume();
        else
            jobRunner.start(); });
    homeScreen.setOnStop([this]()
                         { jobRunner.stop(); });

    homeScreen.setOnFraming([this]()
                            { framingRunner.start(
                                  homeScreen.getProjectMinX(), homeScreen.getProjectMinY(),
                                  homeScreen.getProjectMaxX(), homeScreen.getProjectMaxY(),
                                  appSettings.getFramingFeedRate()); });

    filesScreen.setOnFileSelected([this](const String &path) {
        pendingFilePath = path;
        confirmTarget = ConfirmModalTarget::LoadFile;

        int lastSlash = path.lastIndexOf('/');
        String filename = (lastSlash >= 0) ? path.substring(lastSlash + 1) : path;

        confirmModal.show(tr(StringId::App_Open_File) + filename + "?");
    });

    settingsScreen.setOnLanguageChanged([this]() {
        confirmTarget = ConfirmModalTarget::LanguageRestart;
        confirmModal.show(tr(StringId::App_Modal_Language));
    });

    confirmModal.setOnConfirm([this]() {
        if (confirmTarget == ConfirmModalTarget::LoadFile)
        {
            loadingModal.show(tr(StringId::App_Loading_File));
            loadingModal.draw(display);

            homeScreen.loadJob(pendingFilePath);
            jobRunner.load(pendingFilePath, homeScreen.getTotalLines());

            loadingModal.hide();
            screenManager.switchToScreen(0);
            screenManager.redrawAll();
        }
        else if (confirmTarget == ConfirmModalTarget::LanguageRestart)
        {
            loadingModal.show(tr(StringId::App_Reset));
            loadingModal.draw(display);
            delay(1500);
            ESP.restart();
        }
    });

    confirmModal.setOnCancel([this]()
                             { screenManager.redrawAll(); });

    screenManager.showInitialScreen(0);
    screenManager.setSdStatus(sdReady);
    screenManager.setWifiStatus(false);

    screenManager.draw(display);
}

void App::update()
{
    TouchEvent event = touch.poll();
    jobRunner.update();
    framingRunner.update();
    wifiManager.update();
    screenManager.setWifiStatus(wifiManager.getMode() == WifiMode::Connected);
    screenManager.setMachineStatus(grblController.getConnectionState() == GrblConnectionState::Connected);
    homeScreen.updateMachineState(
        jobRunner.getState(),
        grblController.getStatus(),
        jobRunner.getCurrentLine(),
        jobRunner.getTotalLines());

    if (confirmModal.isVisible())
        confirmModal.handleTouch(event);
    else if (loadingModal.isVisible())
        loadingModal.handleTouch(event);
    else if (settingsScreen.isKeyboardVisible())
    {
        settingsScreen.handleKeyboardTouch(event);
        if (!settingsScreen.isKeyboardVisible())
            screenManager.redrawAll();
    }
    else if (settingsScreen.isNumericPadVisible())
    {
        settingsScreen.handleNumericPadTouch(event);
        if (!settingsScreen.isNumericPadVisible())
            screenManager.redrawAll();
    }
    else if (settingsScreen.isEnumPickerVisible())
    {
        settingsScreen.handleEnumPickerTouch(event);
        if (!settingsScreen.isEnumPickerVisible())
            screenManager.redrawAll();
    }
    else
    {
        screenManager.handleTouch(event);
        screenManager.update();
    }

    screenManager.draw(display);
    confirmModal.draw(display);
    loadingModal.draw(display);

    if (settingsScreen.isKeyboardVisible())
        settingsScreen.drawKeyboard(display);
    if (settingsScreen.isNumericPadVisible())
        settingsScreen.drawNumericPad(display);
    if (settingsScreen.isEnumPickerVisible())
        settingsScreen.drawEnumPicker(display);
}