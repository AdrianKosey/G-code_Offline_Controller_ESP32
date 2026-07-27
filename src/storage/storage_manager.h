#pragma once

#include "istorage_driver.h"
#include "sd_storage_driver.h"
#include "usb_storage_driver.h"

enum class StorageSource { SD, USB };

class StorageManager
{
public:
    StorageManager(uint8_t sdCsPin, uint8_t usbCsPin, uint8_t usbIntPin);

    void begin();
    void update();

    bool isSdAvailable() const;
    bool isUsbAvailable() const;

    IStorageDriver& getDriver(StorageSource source);

private:
    SdStorageDriver sdDriver;
    UsbStorageDriver usbDriver;

    bool sdAvailable = false;
    bool usbAvailable = false;

    unsigned long lastCheckAt = 0;
    static constexpr unsigned long CHECK_INTERVAL_MS = 1000;
};