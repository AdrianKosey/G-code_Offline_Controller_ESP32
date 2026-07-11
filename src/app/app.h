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

#include "../machine/grbl_controller.h"
#include "../gcode/gcode_job_runner.h"
class App
{
public:
    App();
    void begin();
    void update();

private:
    bool sdReady = false;
    GrblController grblController;
    GCodeJobRunner jobRunner;
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