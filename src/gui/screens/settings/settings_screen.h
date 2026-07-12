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
#include "../../widgets/grbl_settings_list/grbl_settings_list_widget.h"
#include "../../widgets/numeric_pad/numeric_pad_modal_widget.h"
#include "../../widgets/enum_picker/enum_picker_modal_widget.h"
#include "../../../machine/grbl_controller.h"

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

    SettingsScreen(WifiManager& wifiManager, GrblController& grblController);
    void draw(DisplayManager& display) override;
    void update() override;

    // WIFI
    bool isKeyboardVisible() const { return passwordKeyboard.isVisible(); }
    void drawKeyboard(DisplayManager& display) { passwordKeyboard.draw(display); }
    bool handleKeyboardTouch(const TouchEvent& event) { return passwordKeyboard.handleTouch(event); }

    // MACHINE
    bool isNumericPadVisible() const { return numericPad.isVisible(); }
    void drawNumericPad(DisplayManager& display) { numericPad.draw(display); }
    bool handleNumericPadTouch(const TouchEvent& event) { return numericPad.handleTouch(event); }

    bool isEnumPickerVisible() const { return enumPicker.isVisible(); }
    void drawEnumPicker(DisplayManager& display) { enumPicker.draw(display); }
    bool handleEnumPickerTouch(const TouchEvent& event) { return enumPicker.handleTouch(event); }

private:
    GrblController& grbl;
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
    GrblSettingsListWidget grblSettingsList;
    NumericPadModalWidget numericPad;
    EnumPickerModalWidget enumPicker;

    uint8_t editingSettingIndex = 0;

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