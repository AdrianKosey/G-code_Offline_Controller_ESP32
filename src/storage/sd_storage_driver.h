#pragma once

#include <SD.h>
#include "istorage_driver.h"

class SdStorageFile : public IStorageFile
{
public:
    SdStorageFile(File file) : file(file) {}

    bool isValid() const override { return (bool)file; }
    bool available() override { return file.available(); }
    String readStringUntil(char terminator) override { return file.readStringUntil(terminator); }
    size_t write(const uint8_t* buffer, size_t size) override { return file.write(buffer, size); }
    size_t read(uint8_t* buffer, size_t size) override { return file.read(buffer, size); }
    uint32_t size() const override { return file.size(); }
    void close() override { file.close(); }

private:
    File file;
};

class SdStorageDriver : public IStorageDriver
{
public:
    SdStorageDriver(uint8_t csPin) : csPin(csPin) {}

    bool begin() override
    {
        available = SD.begin(csPin, SPI, 4000000);
        return available;
    }

    bool isAvailable() override
    {
        available = (SD.cardType() != CARD_NONE);
        return available;
    }

    std::vector<StorageEntry> listDir(const String& path) override
    {
        std::vector<StorageEntry> entries;

        File dir = SD.open(path);
        if (!dir || !dir.isDirectory())
            return entries;

        File file = dir.openNextFile();
        while (file)
        {
            entries.push_back({ String(file.name()), file.isDirectory() });
            file = dir.openNextFile();
        }

        dir.close();
        return entries;
    }

    IStorageFile* openRead(const String& path) override
    {
        File f = SD.open(path, FILE_READ);
        if (!f) return nullptr;
        return new SdStorageFile(f);
    }

    IStorageFile* openWrite(const String& path) override
    {
        File f = SD.open(path, FILE_WRITE);
        if (!f) return nullptr;
        return new SdStorageFile(f);
    }

    bool remove(const String& path) override { return SD.remove(path); }
    bool rename(const String& oldPath, const String& newPath) override { return SD.rename(oldPath, newPath); }
    bool mkdir(const String& path) override { return SD.mkdir(path); }
    bool rmdir(const String& path) override { return SD.rmdir(path); }

    uint64_t totalBytes() override { return SD.cardSize(); }
    const char* driverName() const override { return "SD"; }

private:
    uint8_t csPin;
    bool available = false;
};