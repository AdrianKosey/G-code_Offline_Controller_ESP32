#include "settings_screen.h"

static constexpr int16_t CONTENT_X = 60;
static constexpr int16_t CONTENT_Y = 34;
static constexpr int16_t CONTENT_WIDTH = 260;
static constexpr int16_t CONTENT_HEIGHT = 206;

SettingsScreen::SettingsScreen(WifiManager& wifiManager)
    : // Menu principal
      aboutRow(Rect{CONTENT_X + 8, CONTENT_Y + 8, 244, 40}, "Acerca del dispositivo"),
      wifiRow(Rect{CONTENT_X + 8, CONTENT_Y + 52, 244, 40}, "Wi-Fi"),
      machineRow(Rect{CONTENT_X + 8, CONTENT_Y + 96, 244, 40}, "Machine"),

      // Acerca del dispositivo
      aboutBackButton(
          Rect{CONTENT_X + 8, CONTENT_Y + 4, 28, 24},
          Icons::Back, Icons::HEADER_WIDTH, Icons::HEADER_HEIGHT,
          Theme::Background, Theme::Text, false),
      aboutTitleLabel(Rect{CONTENT_X + 44, CONTENT_Y + 4, 150, 20}, "Acerca del dispositivo", Theme::Text, 2, Theme::Background, false),
      firmwareLabel(Rect{CONTENT_X + 16, CONTENT_Y + 40, 120, 14}, "Firmware", Theme::TextSecondary, 1, Theme::Background, false),
      firmwareValue(Rect{CONTENT_X + 16, CONTENT_Y + 54, 200, 18}, "v0.1.0", Theme::Text, 2, Theme::Background, false),
      projectLabel(Rect{CONTENT_X + 16, CONTENT_Y + 82, 120, 14}, "Proyecto", Theme::TextSecondary, 1, Theme::Background, false),
      projectValue(Rect{CONTENT_X + 16, CONTENT_Y + 96, 220, 18}, "G-code Offline Controller", Theme::Text, 2, Theme::Background, true),

      // Wi-Fi
      wifiBackButton(
          Rect{CONTENT_X + 8, CONTENT_Y + 4, 28, 24},
          Icons::Back, Icons::HEADER_WIDTH, Icons::HEADER_HEIGHT,
          Theme::Background, Theme::Text, false),
      wifiTitleLabel(Rect{CONTENT_X + 44, CONTENT_Y + 4, 100, 20}, "Wi-Fi", Theme::Text, 2, Theme::Background, false),
      wifiStatusLabel(Rect{CONTENT_X + 16, CONTENT_Y + 30, 320, 18}, "No conectado", Theme::TextSecondary, 2, Theme::Background, false),
      wifiScanButton(Rect{CONTENT_X + 150, CONTENT_Y-5, 100, 32}, "Buscar redes"),
      forgetNetworkButton(Rect{CONTENT_X + 150, CONTENT_Y-5, 100, 32}, "Olvidar red"),
      // Machine
      machineBackButton(
          Rect{CONTENT_X + 8, CONTENT_Y + 4, 28, 24},
          Icons::Back, Icons::HEADER_WIDTH, Icons::HEADER_HEIGHT,
          Theme::Background, Theme::Text, false),
      machineTitleLabel(Rect{CONTENT_X + 44, CONTENT_Y + 4, 150, 20}, "Machine", Theme::Text, 2, Theme::Background, false),

      invertXLabel(Rect{CONTENT_X + 16, CONTENT_Y + 42, 150, 20}, "Invertir eje X", Theme::Text, 2, Theme::Background, false),
      invertXToggle(Rect{CONTENT_X + 200, CONTENT_Y + 40, 48, 24}, false),

      invertYLabel(Rect{CONTENT_X + 16, CONTENT_Y + 74, 150, 20}, "Invertir eje Y", Theme::Text, 2, Theme::Background, false),
      invertYToggle(Rect{CONTENT_X + 200, CONTENT_Y + 72, 48, 24}, false),

      invertZLabel(Rect{CONTENT_X + 16, CONTENT_Y + 106, 150, 20}, "Invertir eje Z", Theme::Text, 2, Theme::Background, false),
      invertZToggle(Rect{CONTENT_X + 200, CONTENT_Y + 104, 48, 24}, false),

      softLimitsLabel(Rect{CONTENT_X + 16, CONTENT_Y + 138, 150, 20}, "Limites de software", Theme::Text, 2, Theme::Background, false),
      softLimitsToggle(Rect{CONTENT_X + 200, CONTENT_Y + 136, 48, 24}, true),

      wifi(wifiManager),
      networkList(Rect{CONTENT_X + 8, CONTENT_Y + 80, 244, 120}, 34),
      wifiDetailLabel(Rect{CONTENT_X + 16, CONTENT_Y + 58, 220, 16}, "", Theme::TextSecondary, 1, Theme::Background, true),
      passwordKeyboard(Rect{0, 0, 320, 240})
{
    aboutRow.setOnPress([this]() { switchToPage(SettingsPage::About); });
    wifiRow.setOnPress([this]()  { switchToPage(SettingsPage::Wifi); });
    machineRow.setOnPress([this]() { switchToPage(SettingsPage::Machine); });

    aboutBackButton.setOnPress([this]()   { switchToPage(SettingsPage::Menu); });
    wifiBackButton.setOnPress([this]()    { switchToPage(SettingsPage::Menu); });
    machineBackButton.setOnPress([this]() { switchToPage(SettingsPage::Menu); });

    wifiScanButton.setOnPress([this]() {
        wifi.startScan();
        wifiStatusLabel.setText("Buscando redes...");
    });
    forgetNetworkButton.setOnPress([this]() {
        wifi.forgetSavedNetwork();
        wifiStatusLabel.setText("Modo punto de acceso");
        wifiDetailLabel.setText("");
        networkList.clear();
    });
    networkList.setOnSelect([this](const WifiNetworkEntry& entry) {
        if (entry.encrypted)
        {
            pendingSsid = entry.ssid;
            passwordKeyboard.show("Contrasena para " + entry.ssid);
        }
        else
        {
            wifi.connect(entry.ssid, "", true);
            wifiStatusLabel.setText("Conectando...");
            wifiDetailLabel.setText(entry.ssid);
        }
    });

    passwordKeyboard.setOnSubmit([this](const String& password) {
        wifi.connect(pendingSsid, password, true);
        wifiStatusLabel.setText("Conectando a " + pendingSsid + "...");
    });

    passwordKeyboard.setOnCancel([this]() {});

    invertXToggle.setOnChange([](bool) {});
    invertYToggle.setOnChange([](bool) {});
    invertZToggle.setOnChange([](bool) {});
    softLimitsToggle.setOnChange([](bool) {});

    menuWidgets = { &aboutRow, &wifiRow, &machineRow };

    aboutWidgets = {
        &aboutBackButton, &aboutTitleLabel,
        &firmwareLabel, &firmwareValue,
        &projectLabel, &projectValue
    };

    wifiWidgets = {
        &wifiBackButton, &wifiTitleLabel,
        &wifiStatusLabel, &wifiDetailLabel,
        &networkList
    };

    machineWidgets = {
        &machineBackButton, &machineTitleLabel,
        &invertXLabel, &invertXToggle,
        &invertYLabel, &invertYToggle,
        &invertZLabel, &invertZToggle,
        &softLimitsLabel, &softLimitsToggle
    };

    widgets = menuWidgets;
}

void SettingsScreen::switchToPage(SettingsPage page)
{
    currentPage = page;

    if (page == SettingsPage::Wifi)
        wifiWidgetsNeedRebuild = true;

    switch (page)
    {
        case SettingsPage::Menu:    widgets = menuWidgets;    break;
        case SettingsPage::About:   widgets = aboutWidgets;   break;
        case SettingsPage::Wifi:    widgets = wifiWidgets;    break;
        case SettingsPage::Machine: widgets = machineWidgets; break;
    }

    needsClear = true;
    invalidateAll();
}

void SettingsScreen::draw(DisplayManager& display)
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
    const auto& networks = wifi.getNetworks();
    uint8_t count = min((uint8_t)networks.size(), (uint8_t)WifiNetworkListWidget::MAX_ITEMS);

    WifiNetworkEntry entries[WifiNetworkListWidget::MAX_ITEMS];
    for (uint8_t i = 0; i < count; i++)
        entries[i] = { networks[i].ssid, networks[i].rssi, networks[i].encrypted };

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

    WifiMode currentMode = wifi.getMode();

    if (currentMode == WifiMode::Connected)
    {
        wifiStatusLabel.setText("Conectado");
        wifiDetailLabel.setText(wifi.getConnectedSSID() + "  " + wifi.getIP());
    }
    else if (currentMode == WifiMode::Connecting)
    {
        wifiStatusLabel.setText("Conectando...");
    }
    else if (currentMode == WifiMode::AccessPoint)
    {
        wifiStatusLabel.setText("Modo punto de acceso");
        wifiDetailLabel.setText(String("CNC-Controller  ") + wifi.getIP());
    }
    else if (currentMode == WifiMode::Scanning)
    {
        wifiStatusLabel.setText("Buscando redes");
    }
    if ((currentMode != lastWifiMode || wifiWidgetsNeedRebuild) && currentPage == SettingsPage::Wifi)
    {
        wifiWidgets = { &wifiBackButton, &wifiTitleLabel, &wifiStatusLabel, &wifiDetailLabel };

        if (currentMode == WifiMode::Connected)
            wifiWidgets.push_back(&forgetNetworkButton);
        else
            wifiWidgets.push_back(&wifiScanButton);

        wifiWidgets.push_back(&networkList);

        widgets = wifiWidgets;
        invalidateAll();

        wifiWidgetsNeedRebuild = false;
    }

    lastWifiMode = currentMode;
}