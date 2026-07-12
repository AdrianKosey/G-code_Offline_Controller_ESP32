#pragma once

#include <vector>

#include "../../core/screen.h"
#include "../../theme.h"
#include "../../icons.h"
#include "../../widgets/label/label_widget.h"
#include "../../widgets/button/button_widget.h"
#include "../../widgets/icon_button/icon_button_widget.h"
#include "../../widgets/toggle/toggle_widget.h"
#include "../../widgets/menu_row/menu_row_widget.h"
#include "../../widgets/keyboard/keyboard_modal_widget.h"
#include "../../../network/wifi_manager.h"
#include "../../widgets/wifi_list/wifi_network_list_widget.h"
enum class SettingsPage
{
    Menu,
    About,
    Wifi,
    Machine
};

class SettingsScreen : public IScreen
{
public:
    void draw(DisplayManager& display) override;
    void update() override;
    SettingsScreen(WifiManager& wifiManager);

    bool isKeyboardVisible() const { return passwordKeyboard.isVisible(); }
    void drawKeyboard(DisplayManager& display) { passwordKeyboard.draw(display); }
    bool handleKeyboardTouch(const TouchEvent& event) { return passwordKeyboard.handleTouch(event); }
private:
    SettingsPage currentPage = SettingsPage::Menu;
    bool needsClear = false;

    // Main Menu
    MenuRowWidget aboutRow;
    MenuRowWidget wifiRow;
    MenuRowWidget machineRow;

    // About the device
    IconButtonWidget aboutBackButton;
    LabelWidget aboutTitleLabel;
    LabelWidget firmwareLabel, firmwareValue;
    LabelWidget projectLabel, projectValue;

    // Wi-Fi
    IconButtonWidget wifiBackButton;
    LabelWidget wifiTitleLabel;
    LabelWidget wifiStatusLabel;
    ButtonWidget wifiScanButton;
    ButtonWidget forgetNetworkButton;

    // Machine
    IconButtonWidget machineBackButton;
    LabelWidget machineTitleLabel;
    LabelWidget invertXLabel;
    ToggleWidget invertXToggle;
    LabelWidget invertYLabel;
    ToggleWidget invertYToggle;
    LabelWidget invertZLabel;
    ToggleWidget invertZToggle;
    LabelWidget softLimitsLabel;
    ToggleWidget softLimitsToggle;

    std::vector<Widget*> menuWidgets;
    std::vector<Widget*> aboutWidgets;
    std::vector<Widget*> wifiWidgets;
    std::vector<Widget*> machineWidgets;

    void switchToPage(SettingsPage page);

    WifiManager& wifi;

    static constexpr uint8_t MAX_NETWORK_ROWS = 8;
    WifiNetworkListWidget networkList;
    String pendingSsid;
    LabelWidget wifiDetailLabel;

    KeyboardModalWidget passwordKeyboard;
    WifiMode lastWifiMode = WifiMode::Disconnected;
    bool wifiWidgetsNeedRebuild = true;

    void refreshNetworkList();
};