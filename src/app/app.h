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
#include "../network/wifi_manager.h"
#include "../machine/grbl_controller.h"
#include "../gcode/gcode_job_runner.h"
#include "../machine/framing_runner.h"
#include "app_settings_manager.h"
#include "../i18n/translations.h"
enum class ConfirmModalTarget { LoadFile, LanguageRestart };
class App
{
public:
    App();
    void begin();
    void update();

private:
    AppSettingsManager appSettings;
    ConfirmModalTarget confirmTarget = ConfirmModalTarget::LoadFile;
    bool sdReady = false;
    WifiManager wifiManager;
    GrblController grblController;
    GCodeJobRunner jobRunner;
    FramingRunner framingRunner;
    ILI9341Driver displayDriver;
    DisplayManager display;
    TouchManager touch;
    ScreenManager screenManager;

    HomeScreen homeScreen;
    FilesScreen filesScreen;
    JogScreen jogScreen;
    ToolsScreen toolsScreen;
    SettingsScreen settingsScreen;

    ConfirmModalWidget confirmModal;
    LoadingModalWidget loadingModal;
    String pendingFilePath;
};