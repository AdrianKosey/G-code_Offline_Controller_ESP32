#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <SD.h>

struct DeviceInfo
{
    float temperatureC = 0;
    unsigned long uptimeSeconds = 0;

    uint32_t heapTotal = 0;
    uint32_t heapFree = 0;

    uint32_t cpuFreqMHz = 0;

    int32_t wifiRssi = 0;
    bool wifiConnected = false;
    String ip;
    String mac;

    uint64_t sdTotalBytes = 0;
    uint64_t sdUsedBytes = 0;
    bool sdAvailable = false;
};

class DeviceInfoProvider
{
public:
    static DeviceInfo collect(bool includeSdUsage);

private:
    static uint64_t calculateUsedBytes(File dir);
};