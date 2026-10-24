#include "App.h"

static constexpr uint8_t SIDEBAR_WIDTH = 60;
static constexpr int16_t HEADER_HEIGHT = 30;
static constexpr int16_t SCREEN_HEIGHT = 240;

static SidebarItem sidebarItemsBuffer[5];

static const SidebarItem *buildSidebarItems()
{
    sidebarItemsBuffer[0] = {IconId::Home, tr(StringId::Sidebar_Home)};
    sidebarItemsBuffer[1] = {IconId::Files, tr(StringId::Sidebar_Files)};
    sidebarItemsBuffer[2] = {IconId::Prepare, tr(StringId::Sidebar_Jog)};
    sidebarItemsBuffer[3] = {IconId::Tools, tr(StringId::Sidebar_Tools)};
    sidebarItemsBuffer[4] = {IconId::Settings, tr(StringId::Sidebar_Settings)};

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
      infoModal(Rect{0, 0, 320, 240}),
      jobRunner(grblController),
      storageManager(SD_CS_PIN, CH376_CS_PIN, CH376_INT_PIN),
      homeScreen(grblController),
      filesScreen(storageManager),
      jogScreen(grblController, appSettings),
      toolsScreen(grblController),
      framingRunner(grblController),
      settingsScreen(wifiManager, grblController, appSettings, storageManager),
      screenSleep(display, appSettings),
      backlight(display),
      webServer(grblController, jobRunner, framingRunner, appSettings, storageManager, homeScreen),
      gcodeConsole(Rect{0, 0, 320, 240}, grblController)
{
    g_appSettings = &appSettings;
}

void App::begin()
{
    grblController.begin(Serial2, 115200, GRBL_RX_PIN, GRBL_TX_PIN);
    storageManager.begin();
    //grblController.beginSimulated();
    display.begin();

    constexpr bool FORCE_TOUCH_CALIBRATION = false;
    bool touchOk = touch.begin(FORCE_TOUCH_CALIBRATION);


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
        const RecoverySnapshot &snap = jobRecovery.getSnapshot();

        String message = tr(StringId::Work_Interrupted) +
                         String(snap.path) + tr(StringId::Work_Line) + String(snap.line) +
                         tr(StringId::Work_Of) + String(snap.totalLines) + tr(StringId::Work_Resume);

        confirmTarget = ConfirmModalTarget::JobRecovery;
        confirmModal.show(display, message);
    }

    homeScreen.setOnPlayPause([this]()
                              {
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
                                  } });
    homeScreen.setOnStop([this]()
                         {
        jobRunner.stop();
        jobRecovery.clear(); });

    homeScreen.setOnFraming([this](){
        if (!homeScreen.hasValidProjectBounds())
        {
            infoModal.show(display, tr(StringId::Modal_Framing_Error));
            return;
        }
        float powerTool = 0.0f;
        if (toolsScreen.getToolManager().getActiveIndex() >= 0)
        {
            ToolProfile tool = toolsScreen.getToolManager().getActiveTool();
            powerTool = tool.defaultPower;
        }
        framingRunner.start(
            homeScreen.getProjectMinX(), homeScreen.getProjectMinY(),
            homeScreen.getProjectMaxX(), homeScreen.getProjectMaxY(),
            appSettings.getFramingFeedRate(), powerTool); });

    filesScreen.setOnFileSelected([this](const String &path)
                                  {
        pendingFilePath = path;
        pendingSource = filesScreen.getCurrentSource();
        confirmTarget = ConfirmModalTarget::LoadFile;

        int lastSlash = path.lastIndexOf('/');
        String filename = (lastSlash >= 0) ? path.substring(lastSlash + 1) : path;

        confirmModal.show(display, tr(StringId::App_Open_File) + filename + "?"); });

    screenManager.setOnConsolePress([this]()
                                    { gcodeConsole.show(display); });

    settingsScreen.setOnBuzzerChanged([this](bool enabled)
                                      { buzzer.setEnabled(enabled); });

    settingsScreen.setOnLanguageChanged([this]()
                                        {
        confirmTarget = ConfirmModalTarget::LanguageRestart;
        confirmModal.show(display, tr(StringId::App_Modal_Language)); });

    confirmModal.setOnConfirm([this](){
        if (confirmTarget == ConfirmModalTarget::LoadFile)
        {
            loadingModal.show(tr(StringId::App_Loading_File));
            loadingModal.draw(display);

            IStorageDriver &driver = storageManager.getDriver(pendingSource);

            homeScreen.loadJob(driver, pendingFilePath, appSettings.isGcodePreviewEnabled(), appSettings.isFramingEnabled());
            if (jobRunner.load(driver, pendingFilePath, homeScreen.getTotalLines()))
                webServer.setActiveFile(pendingSource, pendingFilePath);

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
            const RecoverySnapshot &snap = jobRecovery.getSnapshot();

            loadingModal.show(tr(StringId::Work_Resuming));
            loadingModal.draw(display);

            // Mandatory Homing - I NEVER trust the saved position without actual physical reference
            grblController.home();

            // Explicitly reapply all modal state where commands like F/G90 are only sent once in the original file
            String setup = String("G") + (snap.metric ? "21" : "20") +
                        " G" + (snap.absoluteMode ? "90" : "91") +
                        " G" + String(snap.plane) +
                        " G" + String(snap.coordSystem);
            grblController.sendLine(setup);

            if (snap.feedRate > 0)
                grblController.sendLine("F" + String(snap.feedRate, 0));

            // Reposition FIRST, with the tool still OFF - rise to safe height, move XY, then lower to real Z.
            // WARNING: This assumes the part/material is still in the same physical position
            // as before the power outage - visually verify before confirming this step.
            float safeZ = appSettings.getSafeZHeight();

            grblController.sendLine("G90 G0 Z" + String(safeZ, 3));
            grblController.sendLine("G0 X" + String(snap.x, 3) + " Y" + String(snap.y, 3));
            grblController.sendLine("G1 Z" + String(snap.z, 3) + " F" + String(min(snap.feedRate, 300.0f), 0));

            
            bool isLaser = false;

            if (toolsScreen.getToolManager().getActiveIndex() >= 0)
            {
                ToolProfile tool = toolsScreen.getToolManager().getActiveTool();
                isLaser = (tool.type == ToolType::Laser);
            }

            if (snap.spindleState == 1)
                grblController.sendLine("M3 S" + String(snap.spindleSpeed, 0));
            else if (snap.spindleState == 2)
                grblController.sendLine("M4 S" + String(snap.spindleSpeed, 0));

            if ((snap.spindleState == 1 || snap.spindleState == 2) && isLaser && grblController.isLaserMode())
            {
                grblController.sendLine("G91 G1 X0 Y0 F1");
                grblController.sendLine("G90");
            }

            // and continue streaming the file from the saved line
            jobRunner.resumeFrom(storageManager.getDriver(StorageSource::SD), snap.path, snap.line, snap.totalLines);

            resumingJob = true;

            loadingModal.hide();
            screenManager.switchToScreen(0);
            screenManager.redrawAll();
        }
            else if (confirmTarget == ConfirmModalTarget::DeleteToolConfirm){
                if (pendingDeleteAction){
                    pendingDeleteAction();
                    pendingDeleteAction = nullptr;
                }

                screenManager.redrawAll();
            } 
        });

    confirmModal.setOnCancel([this](){ 
        if (confirmTarget == ConfirmModalTarget::JobRecovery)
            jobRecovery.clear();
        screenManager.redrawAll(); 
    });

    infoModal.setOnClose([this]() { redrawAllIncludingModals(); });

    toolsScreen.setOnRequestDeleteConfirm([this](std::function<void()> confirmAction)
                                          {
        pendingDeleteAction = confirmAction;
        confirmTarget = ConfirmModalTarget::DeleteToolConfirm;
        confirmModal.show(display, tr(StringId::Tool_Delete_Confirm)); });

    webServer.setOnFileSelected([this](StorageSource source, const String &path)
                                {
        pendingFilePath = path;
        pendingSource = source; 
        confirmTarget = ConfirmModalTarget::LoadFile;

        int lastSlash = path.lastIndexOf('/');
        String filename = (lastSlash >= 0) ? path.substring(lastSlash + 1) : path;

        confirmModal.show(display, tr(StringId::App_Open_File) + filename + "?"); });
    
    screenManager.showInitialScreen(0);
    screenManager.setSdStatus(storageManager.isSdAvailable());
    screenManager.setWifiStatus(wifiManager.getMode() == WifiMode::Connected);
    screenManager.setUsbStatus(storageManager.isUsbAvailable());
    buzzer.begin();
    buzzer.setEnabled(appSettings.isBuzzerEnabled());
    backlight.fadeTo(100, 2000);

    String designedByBootItem = String("Designed By Kosey ") + CONTROL_VERSION;
    String usbStorageBootItem = String("Mounting USB Storage version 0x") + String(toupper(storageManager.getUsbChipVersion()), HEX) + "...";
    std::vector<BootItem> bootSequence = {
        {"Starting Kernel & GPIO Setup...", true},
        {usbStorageBootItem.c_str(), storageManager.Ch376Available()},
        {"Mounting SD Card Storage...", storageManager.isSdAvailable()},
        {"Connecting to GRBL Controller...", grblController.getConnectionState() == GrblConnectionState::Connected},
        {"Touch Status...", touchOk},
        {"Force Calibration of Touch Controller...", FORCE_TOUCH_CALIBRATION},
        {"Starting WiFi Manager...", wifiManager.getMode() == WifiMode::Connected || wifiManager.getMode() == WifiMode::AccessPoint || wifiManager.getMode() == WifiMode::Connecting},
        {"Starting Web Server...", true},
        {"Loading Settings...", true},
        {"Loading Tool Profiles...", true},
        {"Loading Job Recovery...", true},
        {"Loading GCode Engine...", true},
        {"Loading GUI...", true},
        {"Starting Main Loop...", true},
        {designedByBootItem.c_str(), true}
    };

    SplashScreen::show(display, buzzer, bootSequence, 5000, [this]() { backlight.update(); });
    display.clear(Theme::Background);

    screenManager.draw(display);
    xTaskCreatePinnedToCore(
        App::gcodeTaskWrapper,
        "GCodeEngine",
        8192, // Memory allocated to the task stack
        this, // Pass the current App instance
        10,   // HIGH priority
        &gcodeTaskHandle,
        0 // Explicitly assigned to CORE 0
    );
}

void App::update()
{
    TouchEvent event = touch.poll();
    backlight.update();
    screenSleep.update();
    buzzer.update();
    webServer.update();
    storageManager.update();
    if (screenSleep.handleTouch(event))
    {
        if (screenSleep.consumeRedrawRequest())
            redrawAllIncludingModals();
        return;
    }

    wifiManager.update();

    screenManager.setWifiStatus(wifiManager.getMode() == WifiMode::Connected);
    screenManager.setMachineStatus(grblController.getConnectionState() == GrblConnectionState::Connected);
    if (grblController.getConnectionState() != GrblConnectionState::Connected)
    {
        laserModeSynced = false; // If disconnected, we re-synchronize upon reconnection.
    }
    else if (!laserModeSynced && grblController.isSettingsLoaded())
    {
        // The machine's actual $$ settings are now in place: adjust $32 according to the active tool type.
        toolsScreen.syncLaserModeWithActiveTool();
        laserModeSynced = true;
    }
    static bool lastUsbState = storageManager.isUsbAvailable();
    static bool lastSdState = storageManager.isSdAvailable();
    bool currentUsbState = storageManager.isUsbAvailable();
    bool currentSdState = storageManager.isSdAvailable();
    if (currentUsbState != lastUsbState || currentSdState != lastSdState) {
        buzzer.beep();
        lastUsbState = currentUsbState;
        lastSdState = currentSdState;
    }

    screenManager.setUsbStatus(currentUsbState);
    screenManager.setSdStatus(currentSdState);
    
    homeScreen.updateMachineState(
        jobRunner.getState(),
        grblController.getStatus(),
        jobRunner.getCurrentLine(),
        jobRunner.getTotalLines(),
        jobRunner.getElapsedSeconds());

    if (jobRunner.getState() == JobState::Running && appSettings.isJobRecoveryEnabled())
    {
        const GCodeState &parserState = jobRunner.getParserState();

        uint8_t spindleStateValue = 0;
        if (parserState.spindle == SpindleState::ClockwiseOn)
            spindleStateValue = 1;
        else if (parserState.spindle == SpindleState::CounterClockwiseOn)
            spindleStateValue = 2;

        jobRecovery.updateProgress(jobRunner.getCurrentLine(), parserState, spindleStateValue);
    }

    JobState currentJobState = jobRunner.getState();

    if (currentJobState == JobState::Completed && lastJobState != JobState::Completed)
    {
        buzzer.playCompletionMelody();

        String message = String(tr(StringId::Job_Completed_Title)) + "\n\n" +
                         homeScreen.getFilename() + "\n" +
                         tr(StringId::Job_Completed_Time) + formatElapsed(jobRunner.getElapsedSeconds()) + "\n" +
                         tr(StringId::Job_Completed_Lines) + String(jobRunner.getTotalLines());

        infoModal.show(display, message);

        jobRecovery.clear();
        int8_t activeTool = toolsScreen.getToolManager().getActiveIndex();
        if (activeTool >= 0)
            toolsScreen.getToolManager().addUsageSeconds(activeTool, jobRunner.getElapsedSeconds());
    }

    lastJobState = currentJobState;

    if (jobRunner.getState() == JobState::Completed)
        jobRecovery.clear();

    bool anyModalVisible =
        infoModal.isVisible() ||
        confirmModal.isVisible() ||
        loadingModal.isVisible() ||
        gcodeConsole.isVisible() ||
        settingsScreen.isKeyboardVisible() ||
        settingsScreen.isNumericPadVisible() ||
        settingsScreen.isEnumPickerVisible() ||
        toolsScreen.isKeyboardVisible() ||
        toolsScreen.isNumericPadVisible();

    bool wasConsoleVisible = gcodeConsole.isVisible();

    gcodeConsole.update();

    bool consumed = false;
    if(infoModal.isVisible()){
        consumed = infoModal.handleTouch(event);
    }
    else if (gcodeConsole.isVisible())
    {
        consumed = gcodeConsole.handleTouch(event);

        if (wasConsoleVisible && !gcodeConsole.isVisible())
            screenManager.redrawAll();
    }
    else if (confirmModal.isVisible())
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
    else if (toolsScreen.isKeyboardVisible())
    {
        consumed = toolsScreen.handleKeyboardTouch(event);
        if (!toolsScreen.isKeyboardVisible())
            screenManager.redrawAll();
    }
    else if (toolsScreen.isNumericPadVisible())
    {
        consumed = toolsScreen.handleNumericPadTouch(event);
        if (!toolsScreen.isNumericPadVisible())
            screenManager.redrawAll();
    }
    else
    {
        consumed = screenManager.handleTouch(event);
        screenManager.update();
    }

    if (event.type == TouchType::Pressed && consumed)
        buzzer.beep();
        
    if(!screenSleep.isSleeping()){
        if (!anyModalVisible)
            screenManager.draw(display);

        confirmModal.draw(display);
        loadingModal.draw(display);
        gcodeConsole.draw(display);
        infoModal.draw(display);

        if (settingsScreen.isKeyboardVisible())
            settingsScreen.drawKeyboard(display);
        if (settingsScreen.isNumericPadVisible())
            settingsScreen.drawNumericPad(display);
        if (settingsScreen.isEnumPickerVisible())
            settingsScreen.drawEnumPicker(display);
        if (toolsScreen.isKeyboardVisible())
            toolsScreen.drawKeyboard(display);
        if (toolsScreen.isNumericPadVisible())
            toolsScreen.drawNumericPad(display);
    }
}

void App::gcodeTaskWrapper(void *pvParameters)
{
    App *app = static_cast<App *>(pvParameters);
    for (;;)
    {
        app->updateGCodeEngine();
        // Mandatory minimum delay to avoid blocking the Core 0 Watchdog
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void App::updateGCodeEngine()
{
    // CNC priority process (Core 0)
    jobRunner.update();
    framingRunner.update();
}

void App::redrawAllIncludingModals()
{
    screenManager.redrawAll();

    confirmModal.requestRedraw();
    loadingModal.requestRedraw();
    infoModal.requestRedraw();
    gcodeConsole.requestRedraw();
    settingsScreen.requestModalsRedraw();
    toolsScreen.requestModalsRedraw();
}
