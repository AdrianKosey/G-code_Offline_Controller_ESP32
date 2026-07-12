#pragma once

#include <WiFi.h>
#include <Preferences.h>
#include <vector>

struct WifiNetwork
{
    String ssid;
    int32_t rssi;
    bool encrypted;
};

enum class WifiMode
{
    Disconnected,
    Scanning,
    Connecting,
    Connected,
    AccessPoint
};

class WifiManager
{
public:
    void begin();

    void update();

    void startScan();
    bool isScanning() const;
    const std::vector<WifiNetwork>& getNetworks() const;

    void connect(const String& ssid, const String& password, bool save = true);
    void forgetSavedNetwork();

    WifiMode getMode() const;
    String getIP() const;
    String getConnectedSSID() const;

private:
    Preferences prefs;

    std::vector<WifiNetwork> networks;
    bool scanning = false;

    WifiMode mode = WifiMode::Disconnected;

    unsigned long connectStartedAt = 0;
    static constexpr unsigned long CONNECT_TIMEOUT_MS = 15000;

    static constexpr const char* AP_SSID = "CNC-Controller";
    static constexpr const char* AP_PASSWORD = "cnc12345";

    void startAccessPoint();
    bool tryStoredCredentials();
};