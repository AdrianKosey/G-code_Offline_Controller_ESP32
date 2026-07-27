#pragma once

#include <Ch376msc.h>
#include "istorage_driver.h"

class UsbStorageFile : public IStorageFile
{
public:
    UsbStorageFile(Ch376msc* chip, uint32_t fileSize)
        : chip(chip), fileSizeCached(fileSize)
    {}

    bool isValid() const override { return valid; }

    bool available() override
    {
        return bytesRead < fileSizeCached;
    }

    String readStringUntil(char terminator) override
    {
        String line;
        char buf[2] = {0, 0};

        // I read byte by byte
        while (bytesRead < fileSizeCached)
        {
            bool more = chip->readFile(buf, 1);
            bytesRead++;

            if (buf[0] == terminator)
                break;

            line += buf[0];

            if (!more)
                break;
        }

        return line;
    }

    size_t write(const uint8_t* buffer, size_t size) override
    {
        return chip->writeFile((char*)buffer, size);
    }

    size_t read(uint8_t* buffer, size_t size) override
    {
        chip->readFile((char*)buffer, size);
        bytesRead += size;
        return size;
    }

    uint32_t size() const override { return fileSizeCached; }

    void close() override
    {
        chip->closeFile();
        valid = false;
    }

private:
    Ch376msc* chip;
    bool valid = true;
    uint32_t bytesRead = 0;
    uint32_t fileSizeCached;
};

class UsbStorageDriver : public IStorageDriver
{
public:
    UsbStorageDriver(uint8_t csPin, uint8_t intPin)
        : chip(csPin, intPin)
    {}

    bool begin() override
    {
        chip.init();
        chip.setSource(0); // 0 = USB (the "1" in this library is an SD slot specific to the CH376 module; it is not used from this chip, it is used from the screen)
        available = chip.driveReady();
        return available;
    }

    bool isAvailable() override
    {
        if (chip.checkIntMessage())
            available = chip.getDeviceStatus();

        return available;
    }

    std::vector<StorageEntry> listDir(const String& path) override
    {
        std::vector<StorageEntry> entries;

        if (!navigateTo(path))
            return entries;

        while (chip.listDir())
        {
            StorageEntry entry;
            entry.name = String(chip.getFileName());
            entry.isDirectory = (chip.getFileAttrb() == CH376_ATTR_DIRECTORY);
            entries.push_back(entry);
        }

        return entries;
    }

    IStorageFile* openRead(const String& path) override
    {
        String dir, filename;
        splitPath(path, dir, filename);

        if (!navigateTo(dir))
            return nullptr;

        chip.setFileName(filename.c_str());

        if (chip.openFile() != ANSW_USB_INT_SUCCESS)
            return nullptr;

        return new UsbStorageFile(&chip, chip.getFileSize());
    }

    IStorageFile* openWrite(const String& path) override
    {
        String dir, filename;
        splitPath(path, dir, filename);

        if (!navigateTo(dir))
            return nullptr;

        chip.setFileName(filename.c_str());
        chip.openFile(); // Creates the file if it doesn't exist

        return new UsbStorageFile(&chip, 0);
    }

    bool remove(const String& path) override
    {
        String dir, filename;
        splitPath(path, dir, filename);

        if (!navigateTo(dir))
            return false;

        chip.setFileName(filename.c_str());
        return chip.deleteFile();
    }

    bool rename(const String& oldPath, const String& newPath) override
    {
        // This library doesn't expose a direct rename command in its public API -
        //It would have to be implemented like copy+delete (read the entire old file,
        // write it with the new name, delete the old one). We'll leave that pending
        // until we confirm if there's a native method I've missed.
        return false;
    }

    bool mkdir(const String& path) override
    {
        // `cd()` with `flag=1` seems to create the folder if it doesn't exist, according to the official example -
        // I can't confirm the hardware version yet; the PCB isn't ready.
        return chip.cd(path.c_str(), 1) == ANSW_USB_INT_SUCCESS;
    }

    bool rmdir(const String& path) override
    {
        return false; // TODO: Implement with chip.deleteDir(path) after confirming its use
    }

    uint64_t totalBytes() override
    {
        return (uint64_t)chip.getTotalSectors() * 512ULL; // SECTORSIZE of the library = 512
    }

    const char* driverName() const override { return "USB"; }

private:
    Ch376msc chip;
    bool available = false;

    void splitPath(const String& fullPath, String& dir, String& filename)
    {
        int lastSlash = fullPath.lastIndexOf('/');

        if (lastSlash <= 0)
        {
            dir = "/";
            filename = fullPath.substring(lastSlash + 1);
        }
        else
        {
            dir = fullPath.substring(0, lastSlash);
            filename = fullPath.substring(lastSlash + 1);
        }
    }

    bool navigateTo(const String& path)
    {
        if (path == "/" || path.length() == 0)
            return true; // root - cd() is not needed

        uint8_t result = chip.cd(path.c_str(), 0); // 0 = do not create, only browse (to be confirmed)
        return (result == ANSW_USB_INT_SUCCESS || result == ANSW_ERR_OPEN_DIR);
    }
};