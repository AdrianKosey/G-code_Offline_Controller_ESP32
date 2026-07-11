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
#include "../gui/screens/prepare/prepare_screen.h"
#include "../gui/screens/settings/settings_screen.h"
#include "../gui/screens/tools/tools_screen.h"
class App
{
public:
    App();
    void begin();
    void update();

private:
    bool sdReady = false;
    ILI9341Driver displayDriver;
    DisplayManager display;
    TouchManager touch;
    ScreenManager screenManager;

    HomeScreen homeScreen;
    FilesScreen filesScreen;
    PrepareScreen prepareScreen;
    ToolsScreen toolsScreen;
    SettingsScreen settingsScreen;
};