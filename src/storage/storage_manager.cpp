#include "storage_manager.h"

StorageManager::StorageManager(uint8_t sdCsPin, uint8_t usbCsPin, uint8_t usbIntPin)
    : sdDriver(sdCsPin), usbDriver(usbCsPin, usbIntPin)
{}

void StorageManager::begin()
{
    sdAvailable = sdDriver.begin();
    usbAvailable = usbDriver.begin();
}

void StorageManager::update()
{
    if (millis() - lastCheckAt < CHECK_INTERVAL_MS)
        return;

    lastCheckAt = millis();

    sdAvailable = sdDriver.isAvailable();
    usbAvailable = usbDriver.isAvailable();
}

bool StorageManager::isSdAvailable() const { return sdAvailable; }
bool StorageManager::isUsbAvailable() const { return usbAvailable; }

IStorageDriver& StorageManager::getDriver(StorageSource source)
{
    return (source == StorageSource::SD) ? (IStorageDriver&)sdDriver : (IStorageDriver&)usbDriver;
}