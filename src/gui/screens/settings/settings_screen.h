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
#include "../../../app/app_settings_manager.h"
#include "../../core/scroll_panel_widget.h"
#include "about_content_builder.h"
enum class SettingsPage
{
    Menu,
    About,
    Wifi,
    Machine,
    Control
};

enum class NumericPadTarget
{
    GrblSetting,
    JogFeed,
    FramingFeed,
    SafeZ,
    ScreenSleepMinutes
};

enum class EnumPickerTarget
{
    GrblSetting,
    Language
};

class SettingsScreen : public IScreen
{
public:

    SettingsScreen(WifiManager& wifiManager, GrblController& grblController, AppSettingsManager& appSettingsManager);
    void onEnter() override;

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

    // Config
    using BoolChangeCallback = std::function<void(bool)>;
    void setOnBuzzerChanged(BoolChangeCallback callback);

    using LanguageChangeCallback = std::function<void()>;
    void setOnLanguageChanged(LanguageChangeCallback callback);

private:
    LanguageChangeCallback onLanguageChanged;
    BoolChangeCallback onBuzzerChanged;
    SettingsPage currentPage = SettingsPage::Menu;
    bool needsClear = false;

    // Main Menu
    MenuRowWidget aboutRow;
    MenuRowWidget wifiRow;
    MenuRowWidget machineRow;
    MenuRowWidget controlRow;

    // About the device
    IconButtonWidget aboutBackButton;
    LabelWidget aboutTitleLabel;
    ScrollPanelWidget aboutScrollPanel;
    AboutContentBuilder aboutContent;

    // Wi-Fi
    IconButtonWidget wifiBackButton;
    LabelWidget wifiTitleLabel;
    LabelWidget wifiStatusLabel;
    ButtonWidget wifiScanButton;
    ButtonWidget forgetNetworkButton;

    // Machine
    IconButtonWidget machineBackButton;
    LabelWidget machineTitleLabel;

    // Control (app settings)
    IconButtonWidget controlBackButton;
    LabelWidget controlTitleLabel;
    MenuRowWidget jogFeedRow;
    MenuRowWidget framingFeedRow;
    LabelWidget previewLabel;
    ToggleWidget previewToggle;
    MenuRowWidget languageRow;
    MenuRowWidget safeZRow;
    ScrollPanelWidget controlScrollPanel;
    LabelWidget jobRecoveryLabel;
    ToggleWidget jobRecoveryToggle;
    LabelWidget framingLabel;
    ToggleWidget framingToggle;
    LabelWidget screenSleepLabel;
    ToggleWidget screenSleepToggle;
    MenuRowWidget screenSleepMinutesRow;
    LabelWidget buzzerLabel;
    ToggleWidget buzzerToggle;

    std::vector<Widget*> menuWidgets;
    std::vector<Widget*> aboutWidgets;
    std::vector<Widget*> wifiWidgets;
    std::vector<Widget*> machineWidgets;
    std::vector<Widget*> controlWidgets;

    void switchToPage(SettingsPage page);
    void refreshNetworkList();

    WifiManager& wifi;

    WifiNetworkListWidget networkList;
    LabelWidget wifiDetailLabel;
    KeyboardModalWidget passwordKeyboard;
    String pendingSsid;

    GrblController& grbl;
    GrblSettingsListWidget grblSettingsList;
    uint8_t editingSettingIndex = 0;

    AppSettingsManager& appSettings;

    NumericPadModalWidget numericPad;
    NumericPadTarget numericPadTarget = NumericPadTarget::GrblSetting;

    EnumPickerModalWidget enumPicker;
    EnumPickerTarget enumPickerTarget = EnumPickerTarget::GrblSetting;
};