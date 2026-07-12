#include "wifi_manager.h"

void WifiManager::begin()
{
    prefs.begin("wifi", false);

    if (!tryStoredCredentials())
        startAccessPoint();
}

bool WifiManager::tryStoredCredentials()
{
    String ssid = prefs.getString("ssid", "");

    if (ssid.length() == 0)
        return false;

    String password = prefs.getString("password", "");

    connect(ssid, password, false); // false = do not save again, it already comes from storage
    return true;
}

void WifiManager::connect(const String& ssid, const String& password, bool save)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.length() ? password.c_str() : nullptr);

    mode = WifiMode::Connecting;
    connectStartedAt = millis();

    if (save)
    {
        prefs.putString("ssid", ssid);
        prefs.putString("password", password);
    }
}

void WifiManager::forgetSavedNetwork()
{
    prefs.remove("ssid");
    prefs.remove("password");
    WiFi.disconnect(true);
    startAccessPoint();
}

void WifiManager::startAccessPoint()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    mode = WifiMode::AccessPoint;
}

void WifiManager::startScan()
{
    // Scanning requires STA mode - if we were in AP mode, we temporarily switched
    if (WiFi.getMode() == WIFI_AP)
        WiFi.mode(WIFI_AP_STA);

    WiFi.scanNetworks(true);
    scanning = true;
    mode = WifiMode::Scanning;
}

bool WifiManager::isScanning() const { return scanning; }
const std::vector<WifiNetwork>& WifiManager::getNetworks() const { return networks; }

WifiMode WifiManager::getMode() const { return mode; }

String WifiManager::getIP() const
{
    return (mode == WifiMode::AccessPoint) ? WiFi.softAPIP().toString() : WiFi.localIP().toString();
}

String WifiManager::getConnectedSSID() const
{
    return WiFi.SSID();
}

void WifiManager::update()
{
    if (scanning)
    {
        int result = WiFi.scanComplete();

        if (result >= 0)
        {
            networks.clear();

            for (int i = 0; i < result; i++)
            {
                networks.push_back({
                    WiFi.SSID(i),
                    WiFi.RSSI(i),
                    WiFi.encryptionType(i) != WIFI_AUTH_OPEN
                });
            }

            WiFi.scanDelete();
            scanning = false;

            mode = (WiFi.status() == WL_CONNECTED) ? WifiMode::Connected : WifiMode::AccessPoint;
        }
        else if (result == WIFI_SCAN_FAILED)
        {
            scanning = false;
            mode = (WiFi.status() == WL_CONNECTED) ? WifiMode::Connected : WifiMode::AccessPoint;
        }

        return;
    }

    if (mode == WifiMode::Connecting)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            mode = WifiMode::Connected;
        }
        else if (millis() - connectStartedAt > CONNECT_TIMEOUT_MS)
        {
            startAccessPoint();
        }
    }
}