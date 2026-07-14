#include "settings_screen.h"
#include "../../../i18n/translations.h"

static constexpr int16_t CONTENT_X = 60;
static constexpr int16_t CONTENT_Y = 34;
static constexpr int16_t CONTENT_WIDTH = 260;
static constexpr int16_t CONTENT_HEIGHT = 206;

static const char *LANGUAGE_OPTIONS[] = {"Espanol", "English"};

SettingsScreen::SettingsScreen(WifiManager &wifiManager, GrblController &grblController, AppSettingsManager &appSettingsManager)
    : // Menu principal
      aboutRow(Rect{CONTENT_X + 8, CONTENT_Y + 8, 244, 40}, tr(StringId::Settings_About)),
      wifiRow(Rect{CONTENT_X + 8, CONTENT_Y + 52, 244, 40}, tr(StringId::Settings_Wifi)),
      machineRow(Rect{CONTENT_X + 8, CONTENT_Y + 96, 244, 40}, tr(StringId::Settings_Machine)),
      controlRow(Rect{CONTENT_X + 8, CONTENT_Y + 140, 244, 40}, tr(StringId::Settings_Control)),

      // Acerca del dispositivo
      aboutBackButton(
          Rect{CONTENT_X + 8, CONTENT_Y + 4, 28, 24},
          Icons::Back, Icons::HEADER_WIDTH, Icons::HEADER_HEIGHT,
          Theme::Background, Theme::Text, false),
      aboutTitleLabel(Rect{CONTENT_X + 44, CONTENT_Y + 4, 150, 20}, tr(StringId::Settings_About), Theme::Text, 2, Theme::Background, false),
      aboutScrollPanel(Rect{CONTENT_X + 8, CONTENT_Y + 40, 244, 160}),
      aboutContent(grblController, wifiManager),

      // Wi-Fi
      wifiBackButton(
          Rect{CONTENT_X + 8, CONTENT_Y + 4, 28, 24},
          Icons::Back, Icons::HEADER_WIDTH, Icons::HEADER_HEIGHT,
          Theme::Background, Theme::Text, false),
      wifiTitleLabel(Rect{CONTENT_X + 44, CONTENT_Y + 4, 100, 20}, tr(StringId::Settings_Wifi), Theme::Text, 2, Theme::Background, false),
      wifiStatusLabel(Rect{CONTENT_X + 16, CONTENT_Y + 30, 320, 18}, tr(StringId::Settings_NotConnected), Theme::TextSecondary, 2, Theme::Background, false),
      wifiScanButton(Rect{CONTENT_X + 150, CONTENT_Y, 100, 32}, tr(StringId::Settings_ScanNetworks)),
      forgetNetworkButton(Rect{CONTENT_X + 150, CONTENT_Y, 100, 32}, tr(StringId::Settings_ForgetNetwork)),

      // Machine
      machineBackButton(
          Rect{CONTENT_X + 8, CONTENT_Y + 4, 28, 24},
          Icons::Back, Icons::HEADER_WIDTH, Icons::HEADER_HEIGHT,
          Theme::Background, Theme::Text, false),
      machineTitleLabel(Rect{CONTENT_X + 44, CONTENT_Y + 4, 150, 20}, tr(StringId::Settings_Machine), Theme::Text, 2, Theme::Background, false),

      // Control
      controlBackButton(
          Rect{CONTENT_X + 8, CONTENT_Y + 4, 28, 24},
          Icons::Back, Icons::HEADER_WIDTH, Icons::HEADER_HEIGHT,
          Theme::Background, Theme::Text, false),
      controlTitleLabel(Rect{CONTENT_X + 44, CONTENT_Y + 4, 150, 20}, tr(StringId::Settings_Control), Theme::Text, 2, Theme::Background, false),
      jogFeedRow(Rect{CONTENT_X + 8, CONTENT_Y + 42, 244, 36}, tr(StringId::Settings_JogSpeed)),
      framingFeedRow(Rect{CONTENT_X + 8, CONTENT_Y + 80, 244, 36}, tr(StringId::Settings_FramingSpeed)),
      safeZRow(Rect{CONTENT_X + 8, CONTENT_Y + 118, 244, 36}, tr(StringId::Settings_SafeZ)),
      previewLabel(Rect{CONTENT_X + 16, CONTENT_Y + 126, 150, 20}, tr(StringId::Settings_GcodePreview), Theme::Text, 2, Theme::Background, false),
      previewToggle(Rect{CONTENT_X + 200, CONTENT_Y + 124, 48, 24}, true),
      framingLabel(Rect{0, 0, 150, 20}, tr(StringId::Settings_Framing), Theme::Text, 2, Theme::Background, false),
      framingToggle(Rect{0, 0, 48, 24}, true),
      jobRecoveryLabel(Rect{0, 0, 150, 20}, tr(StringId::Settings_JobRecovery), Theme::Text, 2, Theme::Background, false),
      jobRecoveryToggle(Rect{0, 0, 48, 24}, false),
      languageRow(Rect{CONTENT_X + 8, CONTENT_Y + 158, 244, 36}, tr(StringId::Settings_Language)),
      controlScrollPanel(Rect{CONTENT_X + 8, CONTENT_Y + 40, 244, 160}),

      // Wifi manager
      wifi(wifiManager),
      networkList(Rect{CONTENT_X + 8, CONTENT_Y + 80, 244, 120}, 34),
      wifiDetailLabel(Rect{CONTENT_X + 16, CONTENT_Y + 58, 220, 16}, "", Theme::TextSecondary, 1, Theme::Background, true),
      passwordKeyboard(Rect{0, 0, 320, 240}),

      // Grbl
      grbl(grblController),
      grblSettingsList(Rect{CONTENT_X + 8, CONTENT_Y + 44, 244, 156}, grbl, 40),

      // App settings
      appSettings(appSettingsManager),

      numericPad(Rect{0, 0, 320, 240}),
      enumPicker(Rect{0, 0, 320, 240})
{
    // MENU DIRECTIONS
    aboutRow.setOnPress([this]()
                        { switchToPage(SettingsPage::About); });
    wifiRow.setOnPress([this]()
                       { switchToPage(SettingsPage::Wifi); });
    machineRow.setOnPress([this]()
                          { switchToPage(SettingsPage::Machine); });
    controlRow.setOnPress([this]()
                          { switchToPage(SettingsPage::Control); });

    aboutBackButton.setOnPress([this]()
                               { switchToPage(SettingsPage::Menu); });
    wifiBackButton.setOnPress([this]()
                              { switchToPage(SettingsPage::Menu); });
    machineBackButton.setOnPress([this]()
                                 { switchToPage(SettingsPage::Menu); });
    controlBackButton.setOnPress([this]()
                                 { switchToPage(SettingsPage::Menu); });

    // About Device
    aboutContent.attachTo(aboutScrollPanel);

    // WIFI

    wifiScanButton.setOnPress([this]()
                              {
        wifi.startScan();
        wifiStatusLabel.setText(tr(StringId::Settings_Scanning)); });

    forgetNetworkButton.setOnPress([this]()
                                   {
        wifi.forgetSavedNetwork();
        wifiStatusLabel.setText(tr(StringId::Settings_AccessPointMode));
        wifiDetailLabel.setText("");
        networkList.clear(); });

    networkList.setOnSelect([this](const WifiNetworkEntry &entry)
                            {
        if (entry.encrypted)
        {
            pendingSsid = entry.ssid;
            passwordKeyboard.show(tr(StringId::Settings_Password) + entry.ssid);
        }
        else
        {
            wifi.connect(entry.ssid, "", true);
            wifiStatusLabel.setText(tr(StringId::Settings_Connecting));
            wifiDetailLabel.setText(entry.ssid);
        } });

    passwordKeyboard.setOnSubmit([this](const String &password)
                                 {
        wifi.connect(pendingSsid, password, true);
        wifiStatusLabel.setText(tr(StringId::Settings_Connecting_To)); 
        wifiDetailLabel.setText(pendingSsid + "..."); });

    passwordKeyboard.setOnCancel([this]() {});

    // Machine
    grblSettingsList.setOnSelect([this](const GrblSettingDef &def, float currentValue)
                                 {
        editingSettingIndex = def.index;

        if (def.type == GrblSettingType::Toggle)
        {
            grbl.setSetting(def.index, currentValue > 0.5f ? 0.0f : 1.0f);
            grblSettingsList.refresh();
        }
        else if (def.type == GrblSettingType::Enum)
        {
            static const char* resolvedLabels[8];
            for (uint8_t i = 0; i < def.enumCount; i++)
                resolvedLabels[i] = tr(def.enumLabelIds[i]);

            enumPickerTarget = EnumPickerTarget::GrblSetting;
            enumPicker.show(tr(def.nameId), resolvedLabels, def.enumCount, (uint8_t)currentValue);
        }
        else
        {
            numericPadTarget = NumericPadTarget::GrblSetting;
            numericPad.show(tr(def.nameId), currentValue);
        } });

    // Control
    jogFeedRow.setValue(String((int)appSettings.getJogFeedRate()) + " mm/min");
    jogFeedRow.setOnPress([this]()
                          {
        numericPadTarget = NumericPadTarget::JogFeed;
        numericPad.show(tr(StringId::Settings_Modal_JogSpeed), appSettings.getJogFeedRate()); });

    framingFeedRow.setValue(String((int)appSettings.getFramingFeedRate()) + " mm/min");
    framingFeedRow.setOnPress([this]()
                              {
        numericPadTarget = NumericPadTarget::FramingFeed;
        numericPad.show(tr(StringId::Settings_Modal_FramingSpeed), appSettings.getFramingFeedRate()); });

    safeZRow.setValue(String(appSettings.getSafeZHeight(), 1) + " mm");
    safeZRow.setOnPress([this]()
                        {
        numericPadTarget = NumericPadTarget::SafeZ;
        numericPad.show(tr(StringId::Settings_SafeZ), appSettings.getSafeZHeight()); });

    previewToggle.setState(appSettings.isGcodePreviewEnabled());
    previewToggle.setOnChange([this](bool state)
                              { appSettings.setGcodePreviewEnabled(state); });

    framingToggle.setState(appSettings.isFramingEnabled());
    framingToggle.setOnChange([this](bool state){
        appSettings.setFramingEnabled(state);

        if (!state)
        {
            appSettings.setGcodePreviewEnabled(false);
            previewToggle.setState(false);
            previewToggle.setEnabled(false);
        }
        else
        {
            previewToggle.setEnabled(true);
        } });
    previewToggle.setEnabled(appSettings.isFramingEnabled());

    jobRecoveryToggle.setState(appSettings.isJobRecoveryEnabled());
    jobRecoveryToggle.setOnChange([this](bool state)
                                  { appSettings.setJobRecoveryEnabled(state); });

    languageRow.setValue(appSettings.getLanguage() == AppLanguage::Spanish ? "Espanol" : "English");
    languageRow.setOnPress([this]()
                           {
        enumPickerTarget = EnumPickerTarget::Language;
        enumPicker.show(tr(StringId::Settings_Language), LANGUAGE_OPTIONS, 2, (uint8_t)appSettings.getLanguage()); });

    controlScrollPanel.addChild(&jogFeedRow, 0, 0);
    controlScrollPanel.addChild(&framingFeedRow, 0, 38);
    controlScrollPanel.addChild(&safeZRow, 0, 38 + 38);
    controlScrollPanel.addChild(&previewLabel, 8, 122);
    controlScrollPanel.addChild(&previewToggle, 192, 120);
    controlScrollPanel.addChild(&framingLabel, 8, 154);
    controlScrollPanel.addChild(&framingToggle, 192, 152);
    controlScrollPanel.addChild(&jobRecoveryLabel, 8, 188);
    controlScrollPanel.addChild(&jobRecoveryToggle, 192, 186);
    controlScrollPanel.addChild(&languageRow, 0, 222);

    // general

    numericPad.setOnSubmit([this](float value)
                           {
        switch (numericPadTarget)
        {
            case NumericPadTarget::GrblSetting:
                grbl.setSetting(editingSettingIndex, value);
                grblSettingsList.refresh();
                break;

            case NumericPadTarget::JogFeed:
                appSettings.setJogFeedRate(value);
                jogFeedRow.setValue(String((int)value) + " mm/min");
                break;

            case NumericPadTarget::FramingFeed:
                appSettings.setFramingFeedRate(value);
                framingFeedRow.setValue(String((int)value) + " mm/min");
                break;

            case NumericPadTarget::SafeZ:
                appSettings.setSafeZHeight(value);
                safeZRow.setValue(String(value, 1) + " mm");
                break;
        } });

    numericPad.setOnCancel([this]() {});

    enumPicker.setOnSubmit([this](uint8_t index)
                           {
        switch (enumPickerTarget)
        {
            case EnumPickerTarget::GrblSetting:
                grbl.setSetting(editingSettingIndex, (float)index);
                grblSettingsList.refresh();
                break;

            case EnumPickerTarget::Language:
                appSettings.setLanguage((AppLanguage)index);
                languageRow.setValue(index == 0 ? "Espanol" : "English");
                if (onLanguageChanged) onLanguageChanged();
                break;
        } });

    // ---------- widgets control--------------

    menuWidgets = {&aboutRow, &wifiRow, &machineRow, &controlRow};

    aboutWidgets = {
        &aboutBackButton, &aboutTitleLabel,
        &aboutScrollPanel
    };

    wifiWidgets = {
        &wifiBackButton, &wifiTitleLabel,
        &wifiStatusLabel, &wifiDetailLabel,
        &networkList};

    machineWidgets = {
        &machineBackButton, &machineTitleLabel,
        &grblSettingsList};

    controlWidgets = {
        &controlBackButton, &controlTitleLabel,
        &controlScrollPanel};

    widgets = menuWidgets;
}

void SettingsScreen::switchToPage(SettingsPage page)
{
    currentPage = page;

    if (page == SettingsPage::About)
        aboutContent.refresh();

    if (page == SettingsPage::Wifi)
    {
        wifiWidgets = {&wifiBackButton, &wifiTitleLabel, &wifiStatusLabel, &wifiDetailLabel};

        if (wifi.getMode() == WifiMode::Connected)
            wifiWidgets.push_back(&forgetNetworkButton);
        else
            wifiWidgets.push_back(&wifiScanButton);

        wifiWidgets.push_back(&networkList);
    }

    switch (page)
    {
    case SettingsPage::Menu:
        widgets = menuWidgets;
        break;
    case SettingsPage::About:
        widgets = aboutWidgets;
        break;
    case SettingsPage::Wifi:
        widgets = wifiWidgets;
        break;
    case SettingsPage::Machine:
        widgets = machineWidgets;
        break;
    case SettingsPage::Control:
        widgets = controlWidgets;
        break;
    }

    needsClear = true;
    invalidateAll();
}

void SettingsScreen::draw(DisplayManager &display)
{
    if (needsClear)
    {
        display.fillRect(CONTENT_X, CONTENT_Y, CONTENT_WIDTH, CONTENT_HEIGHT, Theme::Background);
        needsClear = false;
    }

    IScreen::draw(display);
}

void SettingsScreen::refreshNetworkList()
{
    const auto &networks = wifi.getNetworks();
    uint8_t count = min((uint8_t)networks.size(), (uint8_t)WifiNetworkListWidget::MAX_ITEMS);

    WifiNetworkEntry entries[WifiNetworkListWidget::MAX_ITEMS];
    for (uint8_t i = 0; i < count; i++)
        entries[i] = {networks[i].ssid, networks[i].rssi, networks[i].encrypted};

    networkList.setEntries(entries, count);
}

void SettingsScreen::update()
{
    IScreen::update();

    static bool wasScanning = false;
    bool isScanning = wifi.isScanning();

    if (wasScanning && !isScanning)
        refreshNetworkList();

    wasScanning = isScanning;

    static WifiMode lastMode = WifiMode::Disconnected;
    WifiMode currentMode = wifi.getMode();

    if (currentMode == WifiMode::Connected)
    {
        wifiStatusLabel.setText(tr(StringId::Settings_Connected));
        wifiDetailLabel.setText(wifi.getConnectedSSID() + "  " + wifi.getIP());
    }
    else if (currentMode == WifiMode::Connecting)
    {
        wifiStatusLabel.setText(tr(StringId::Settings_Connecting));
    }
    else if (currentMode == WifiMode::AccessPoint)
    {
        wifiStatusLabel.setText(tr(StringId::Settings_AccessPointMode));
        wifiDetailLabel.setText(String("CNC-Controller  ") + wifi.getIP());
    }
    else if (currentMode == WifiMode::Scanning)
    {
        wifiStatusLabel.setText(tr(StringId::Settings_Scanning));
    }

    if (currentMode != lastMode && currentPage == SettingsPage::Wifi)
    {
        wifiWidgets = {&wifiBackButton, &wifiTitleLabel, &wifiStatusLabel, &wifiDetailLabel};

        if (currentMode == WifiMode::Connected)
            wifiWidgets.push_back(&forgetNetworkButton);
        else
            wifiWidgets.push_back(&wifiScanButton);

        wifiWidgets.push_back(&networkList);

        widgets = wifiWidgets;
        invalidateAll();
    }

    lastMode = currentMode;
}

void SettingsScreen::onEnter()
{
    IScreen::onEnter();
    
    jogFeedRow.setValue(String((int)appSettings.getJogFeedRate()) + " mm/min");
    framingFeedRow.setValue(String((int)appSettings.getFramingFeedRate()) + " mm/min");
    safeZRow.setValue(String(appSettings.getSafeZHeight(), 1) + " mm");
    previewToggle.setState(appSettings.isGcodePreviewEnabled());
    previewToggle.setEnabled(appSettings.isFramingEnabled());
    framingToggle.setState(appSettings.isFramingEnabled());
    jobRecoveryToggle.setState(appSettings.isJobRecoveryEnabled());
    languageRow.setValue(appSettings.getLanguage() == AppLanguage::Spanish ? "Espanol" : "English");
}

void SettingsScreen::setOnLanguageChanged(LanguageChangeCallback callback)
{
    onLanguageChanged = callback;
}