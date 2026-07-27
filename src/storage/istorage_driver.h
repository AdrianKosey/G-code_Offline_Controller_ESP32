#pragma once

#include <Arduino.h>
#include <vector>
#include "istorage_file.h"
#include "storage_entry.h"

class IStorageDriver
{
public:
    virtual ~IStorageDriver() = default;

    virtual bool begin() = 0;
    virtual bool isAvailable() = 0; // It is checked again at runtime (removable/insertable media)

    virtual std::vector<StorageEntry> listDir(const String& path) = 0;

    virtual IStorageFile* openRead(const String& path) = 0;
    virtual IStorageFile* openWrite(const String& path) = 0;

    virtual bool remove(const String& path) = 0;
    virtual bool rename(const String& oldPath, const String& newPath) = 0;
    virtual bool mkdir(const String& path) = 0;
    virtual bool rmdir(const String& path) = 0;

    virtual uint64_t totalBytes() = 0;
    virtual const char* driverName() const = 0; // "SD" or "USB"
};