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
      settingsScreen(wifiManager, grblController, appSettings),
      screenSleep(display, appSettings),
      webServer(grblController, jobRunner, framingRunner, appSettings)
{
    g_appSettings = &appSettings;
}

void App::begin()
{
    grblController.begin(Serial2, 115200, GRBL_RX_PIN, GRBL_TX_PIN);
    //grblController.beginSimulated();
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
    jobRecovery.begin();
    webServer.begin();

    if (appSettings.isJobRecoveryEnabled() && jobRecovery.hasPendingRecovery())
    {
        const RecoverySnapshot& snap = jobRecovery.getSnapshot();

        String message = tr(StringId::Work_Interrupted) +
                          String(snap.path) + tr(StringId::Work_Line) + String(snap.line) +
                          tr(StringId::Work_Of) + String(snap.totalLines) + tr(StringId::Work_Resume);

        confirmTarget = ConfirmModalTarget::JobRecovery;
        confirmModal.show(display ,message);
    }

    homeScreen.setOnPlayPause([this](){
        JobState state = jobRunner.getState();

        if (state == JobState::Running)
        {
            jobRunner.pause();
        }
        else if (state == JobState::Paused)
        {
            jobRunner.resume();
        }
        else
        {
            jobRunner.start();
            if (appSettings.isJobRecoveryEnabled())
                jobRecovery.startJob(pendingFilePath, homeScreen.getTotalLines());
        }
    
    });
    homeScreen.setOnStop([this]() {
        jobRunner.stop();
        jobRecovery.clear();
    });

    homeScreen.setOnFraming([this]() {
        if (!homeScreen.hasValidProjectBounds())
        {
            confirmModal.show(display, tr(StringId::Modal_Framing_Error));
            return;
        }

        framingRunner.start(
            homeScreen.getProjectMinX(), homeScreen.getProjectMinY(),
            homeScreen.getProjectMaxX(), homeScreen.getProjectMaxY(),
            appSettings.getFramingFeedRate());
    });

    filesScreen.setOnFileSelected([this](const String &path) {
        pendingFilePath = path;
        confirmTarget = ConfirmModalTarget::LoadFile;

        int lastSlash = path.lastIndexOf('/');
        String filename = (lastSlash >= 0) ? path.substring(lastSlash + 1) : path;

        confirmModal.show(display, tr(StringId::App_Open_File) + filename + "?");
    });

    settingsScreen.setOnBuzzerChanged([this](bool enabled) {
        buzzer.setEnabled(enabled);
    });

    settingsScreen.setOnLanguageChanged([this]() {
        confirmTarget = ConfirmModalTarget::LanguageRestart;
        confirmModal.show(display, tr(StringId::App_Modal_Language));
    });

    confirmModal.setOnConfirm([this]() {
        if (confirmTarget == ConfirmModalTarget::LoadFile)
        {
            loadingModal.show(tr(StringId::App_Loading_File));
            loadingModal.draw(display);

            homeScreen.loadJob(pendingFilePath, appSettings.isGcodePreviewEnabled(), appSettings.isFramingEnabled()); 

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
        else if (confirmTarget == ConfirmModalTarget::JobRecovery)
        {
            const RecoverySnapshot& snap = jobRecovery.getSnapshot();

            loadingModal.show(tr(StringId::Work_Resuming));
            loadingModal.draw(display);

            //Mandatory Homing - I NEVER trust the saved position without actual physical reference
            grblController.home();

            // Explicitly reapply all modal state where commands like F/G90 are only sent once in the original file
            String setup = String("G") + (snap.metric ? "21" : "20") +
                            " G" + (snap.absoluteMode ? "90" : "91") +
                            " G" + String(snap.plane) +
                            " G" + String(snap.coordSystem);
            grblController.sendLine(setup);

            if (snap.feedRate > 0)
                grblController.sendLine("F" + String(snap.feedRate, 0));

            if (snap.spindleState == 1)
                grblController.sendLine("M3 S" + String(snap.spindleSpeed, 0));
            else if (snap.spindleState == 2)
                grblController.sendLine("M4 S" + String(snap.spindleSpeed, 0));

            // Reposition: First raise to a safe height, then XY, then lower to the actual Z position.
            // WARNING: This assumes the part/material is still in the same physical position
            // as before the power outage - visually verify before confirming this step.
            float safeZ = appSettings.getSafeZHeight();

            grblController.sendLine("G90 G0 Z" + String(safeZ, 3));
            grblController.sendLine("G0 X" + String(snap.x, 3) + " Y" + String(snap.y, 3));
            grblController.sendLine("G1 Z" + String(snap.z, 3) + " F" + String(min(snap.feedRate, 300.0f), 0));

            // and continue streaming the file from the saved line
            jobRunner.resumeFrom(snap.path, snap.line, snap.totalLines);

            resumingJob = true;

            loadingModal.hide();
            screenManager.switchToScreen(0);
            screenManager.redrawAll();
        }
    });

    confirmModal.setOnCancel([this](){ 
        if (confirmTarget == ConfirmModalTarget::JobRecovery)
            jobRecovery.clear();
        screenManager.redrawAll(); 
    });

    webServer.setOnFileSelected([this](const String& path) {
        pendingFilePath = path;
        confirmTarget = ConfirmModalTarget::LoadFile;

        int lastSlash = path.lastIndexOf('/');
        String filename = (lastSlash >= 0) ? path.substring(lastSlash + 1) : path;

        confirmModal.show(display, tr(StringId::App_Open_File) + filename + "?");
    });

    screenManager.showInitialScreen(0);
    screenManager.setSdStatus(sdReady);
    screenManager.setWifiStatus(false);

    buzzer.begin();
    buzzer.setEnabled(appSettings.isBuzzerEnabled());

    screenManager.draw(display);
}

void App::update()
{
    TouchEvent event = touch.poll();

    screenSleep.update();
    buzzer.update();
    webServer.update();

    if (screenSleep.handleTouch(event))
    {
        if (!screenSleep.isSleeping())
            screenManager.redrawAll();
        return;
    }

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

    if (jobRunner.getState() == JobState::Running && appSettings.isJobRecoveryEnabled())
    {
        uint8_t spindleStateValue = 0;
        jobRecovery.updateProgress(jobRunner.getCurrentLine(), jobRunner.getParserState(), spindleStateValue);
    }

    if (jobRunner.getState() == JobState::Completed)
        jobRecovery.clear();

    bool anyModalVisible =
        confirmModal.isVisible() ||
        loadingModal.isVisible() ||
        settingsScreen.isKeyboardVisible() ||
        settingsScreen.isNumericPadVisible() ||
        settingsScreen.isEnumPickerVisible();

    bool consumed = false;

    if (confirmModal.isVisible())
        consumed = confirmModal.handleTouch(event);
    else if (loadingModal.isVisible())
        consumed = loadingModal.handleTouch(event);
    else if (settingsScreen.isKeyboardVisible())
    {
        consumed = settingsScreen.handleKeyboardTouch(event);
        if (!settingsScreen.isKeyboardVisible())
            screenManager.redrawAll();
    }
    else if (settingsScreen.isNumericPadVisible())
    {
        consumed = settingsScreen.handleNumericPadTouch(event);
        if (!settingsScreen.isNumericPadVisible())
            screenManager.redrawAll();
    }
    else if (settingsScreen.isEnumPickerVisible())
    {
        consumed = settingsScreen.handleEnumPickerTouch(event);
        if (!settingsScreen.isEnumPickerVisible())
            screenManager.redrawAll();
    }
    else
    {
        consumed = screenManager.handleTouch(event);
        screenManager.update();
    }

    if (event.type == TouchType::Pressed && consumed)
        buzzer.beep();

    if (!anyModalVisible)
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