#pragma once
#include <SPI.h>
#include <SD.h>
#include <pins.h>
#include "../display/display_manager.h"
#include "../touch/touch_manager.h"
#include "../display/drivers/ILI9341/ILI9341Driver.h"
#include "../gui/core/screen_manager.h"
#include "../gui/screens/home/home_screen.h"
#include "../gui/screens/files/files_screen.h"
#include "../gui/screens/jog/jog_screen.h"
#include "../gui/screens/settings/settings_screen.h"
#include "../gui/screens/tools/tools_screen.h"
#include "../gui/widgets/modal/confirm_modal_widget.h"
#include "../gui/widgets/modal/loading_modal_widget.h"
#include "../gui/widgets/modal/info_modal_widget.h"
#include "../gui/widgets/gcode_console/gcode_console_modal_widget.h"
#include "../network/wifi_manager.h"
#include "../machine/grbl_controller.h"
#include "../gcode/gcode_job_runner.h"
#include "../machine/framing_runner.h"
#include "app_settings_manager.h"
#include "../i18n/translations.h"
#include "../machine/job_recovery_manager.h"
#include "../gui/core/screen_sleep_manager.h"
#include "../gui/core/backlight_fader.h"
#include "../app/buzzer_manager.h"
#include "../network/web_server_manager.h"
#include "../storage/storage_manager.h"
#include "splash_screen.h"
#include "time_format.h"

enum class ConfirmModalTarget { LoadFile, LanguageRestart, JobRecovery, DeleteToolConfirm };
class App
{
public:
    App();
    void begin();
    void update();

private:
    TaskHandle_t gcodeTaskHandle = nullptr;
    void updateGCodeEngine();
    static void gcodeTaskWrapper(void *pvParameters);
    std::function<void()> pendingDeleteAction;
    JobState lastJobState = JobState::Idle;
    AppSettingsManager appSettings;
    ConfirmModalTarget confirmTarget = ConfirmModalTarget::LoadFile;
    StorageManager storageManager;
    StorageSource pendingSource = StorageSource::SD;
    bool sdReady = false;
    WifiManager wifiManager;
    WebServerManager webServer;
    GrblController grblController;
    GCodeJobRunner jobRunner;
    FramingRunner framingRunner;
    ILI9341Driver displayDriver;
    BuzzerManager buzzer;

    DisplayManager display;
    TouchManager touch;
    ScreenManager screenManager;
    ScreenSleepManager screenSleep;
    BacklightFader backlight;

    HomeScreen homeScreen;
    FilesScreen filesScreen;
    JogScreen jogScreen;
    ToolsScreen toolsScreen;
    ToolProfileManager toolManager;
    SettingsScreen settingsScreen;

    JobRecoveryManager jobRecovery;
    bool resumingJob = false;

    ConfirmModalWidget confirmModal;
    LoadingModalWidget loadingModal;
    InfoModalWidget infoModal;
    String pendingFilePath;
    GcodeConsoleModalWidget gcodeConsole;

    bool laserModeSynced = false;
    void redrawAllIncludingModals();
};