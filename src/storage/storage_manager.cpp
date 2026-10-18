#include "storage_manager.h"

StorageManager::StorageManager(uint8_t sdCsPin, uint8_t usbCsPin, uint8_t usbIntPin)
    : sdDriver(sdCsPin), usbDriver(usbCsPin, usbIntPin)
{}

void StorageManager::begin()
{
    ch376Available = usbDriver.begin();
    delay(500);
    sdAvailable = sdDriver.begin();
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
bool StorageManager::Ch376Available() const { return ch376Available; }
IStorageDriver& StorageManager::getDriver(StorageSource source)
{
    return (source == StorageSource::SD) ? (IStorageDriver&)sdDriver : (IStorageDriver&)usbDriver;
}