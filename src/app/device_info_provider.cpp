#include "device_info_provider.h"

DeviceInfo DeviceInfoProvider::collect(bool includeSdUsage)
{
    DeviceInfo info;

    info.temperatureC = temperatureRead(); 
    info.uptimeSeconds = millis() / 1000;

    info.heapTotal = ESP.getHeapSize();
    info.heapFree = ESP.getFreeHeap();

    info.cpuFreqMHz = ESP.getCpuFreqMHz();

    info.wifiConnected = (WiFi.status() == WL_CONNECTED);
    info.wifiRssi = info.wifiConnected ? WiFi.RSSI() : 0;
    info.ip = info.wifiConnected ? WiFi.localIP().toString() : WiFi.softAPIP().toString();
    info.mac = WiFi.macAddress();

    info.sdAvailable = (SD.cardType() != CARD_NONE);

    if (info.sdAvailable)
    {
        info.sdTotalBytes = SD.cardSize();

        if (includeSdUsage)
        {
            File root = SD.open("/");
            info.sdUsedBytes = calculateUsedBytes(root);
            root.close();
        }
    }

    return info;
}

uint64_t DeviceInfoProvider::calculateUsedBytes(File dir)
{
    uint64_t total = 0;
    File entry = dir.openNextFile();

    while (entry)
    {
        if (entry.isDirectory())
            total += calculateUsedBytes(entry);
        else
            total += entry.size();

        entry = dir.openNextFile();
    }

    return total;
}