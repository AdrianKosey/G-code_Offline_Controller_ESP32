#pragma once

#include <vector>
#include <Ch376msc.h>
#include "istorage_driver.h"

class UsbStorageFile : public IStorageFile
{
public:
    UsbStorageFile(Ch376msc *chip, uint32_t fileSize)
        : chip(chip), fileSizeCached(fileSize)
    {
    }

    bool isValid() const override { return valid; }

    bool available() override
    {
        return readOffset < readLength || bytesRead < fileSizeCached;
    }

    String readStringUntil(char terminator) override
    {
        String line;
        while (fillReadBuffer())
        {
            char character = readBuffer[readOffset++];

            if (character == terminator)
                break;

            line += character;
        }

        return line;
    }

    size_t write(const uint8_t *buffer, size_t size) override
    {
        return chip->writeFile((char *)buffer, size);
    }

    size_t read(uint8_t *buffer, size_t size) override
    {
        size_t totalRead = 0;

        while (totalRead < size && fillReadBuffer())
        {
            size_t availableBytes = readLength - readOffset;
            size_t copyLength = min(availableBytes, size - totalRead);
            memcpy(buffer + totalRead, readBuffer + readOffset, copyLength);
            readOffset += copyLength;
            totalRead += copyLength;
        }

        return totalRead;
    }

    uint32_t size() const override { return fileSizeCached; }

    void close() override
    {
        chip->closeFile();
        valid = false;
    }

private:
    Ch376msc *chip;
    bool valid = true;
    uint32_t bytesRead = 0;
    uint32_t fileSizeCached;
    char readBuffer[255];
    uint8_t readOffset = 0;
    uint8_t readLength = 0;

    bool fillReadBuffer()
    {
        if (readOffset < readLength)
            return true;

        if (bytesRead >= fileSizeCached)
            return false;

        // readFile reserves its final buffer byte for '\0', leaving 254 bytes
        // of useful data per CH376 transaction.
        uint8_t requested = (uint8_t)min((uint32_t)254, fileSizeCached - bytesRead);
        chip->readFile(readBuffer, requested + 1);

        readLength = chip->getStreamLen();
        readOffset = 0;
        bytesRead += readLength;

        if (readLength == 0)
        {
            bytesRead = fileSizeCached;
            valid = false;
            return false;
        }

        return true;
    }
};

class UsbStorageDriver : public IStorageDriver
{
public:
    UsbStorageDriver(uint8_t csPin, uint8_t intPin)
        : chip(csPin, intPin, SPI_SCK_MHZ(4))
    {
    }

    bool begin() override
    {
        delay(10000);
        SPI.begin(); 
        chipVersion = 0;
        int attempts = 0;

        // 5 try attempts to initialize the CH376S chip, with a 500ms delay between each attempt 
        // [I hate very much this fucking chip, It's a joke of a chip :)]
        while ((chipVersion == 0 || chipVersion == 0xFF) && attempts < 5) {
            attempts++;
            Serial.print("Attempting to initialize CH376S (Attempt ");
            Serial.print(attempts);
            Serial.println(")...");
            
            chip.init(); 
            delay(100);
            
            chipVersion = chip.getChipVer();
            
            if (chipVersion == 0 || chipVersion == 0xFF) {
                delay(500);
            }
        }

        if (chipVersion != 0 && chipVersion != 0xFF) {
            Serial.print("CH376 successfully initialized! Version: 0x"); // 0x44 is the CH376S version 17/10/2026 test
            Serial.println(chipVersion, HEX);
            
            // USB Host (0)
            chip.setSource(0); 
            delay(100);

            available = chip.driveReady();
            
            if (available) {
                Serial.println("SPI: USB Memory mounted and ready to use.");
            } else {
                Serial.println("SPI: CH376S ready, but no USB flash drive detected yet.");
            }
            return true;
        } else {
            Serial.println("Critical error in CH376S. Requires electrical reset.");
            available = false;
            return false;
        }

    }

    bool isAvailable() override
    {
        if (chip.checkIntMessage()) {
            if (chip.getDeviceStatus()) {
                Serial.println("USB Connected!");
                available = true;
                chip.driveReady();
            } else {
                available = false;
                Serial.println("USB Disconnected.");
            }
    }

        return available;
    }
    
    byte getChipVersion() const { return chipVersion; }

    std::vector<StorageEntry> listDir(const String &path) override
    {
        std::vector<StorageEntry> entries;

        if (!navigateTo(path))
            return entries;

        // listDir() is stateful in Ch376msc.  A previous enumeration can leave
        // its internal state machine waiting for the next entry, so always
        // restart it after changing directory.  Without this, the root can be
        // listed correctly while an immediately selected subdirectory appears
        // empty.
        chip.resetFileList();

        while (chip.listDir())
        {
            StorageEntry entry;
            // Ch376msc exposes the 11 raw bytes of a FAT 8.3 name.  For
            // example, TEST is returned as "TEST       " and JOB.NC as
            // "JOB     NC".  Those bytes cannot be used directly in a path
            // (nor can the latter pass the G-code extension filter).
            entry.name = formatFatName(chip.getFileName());
            entry.isDirectory = (chip.getFileAttrb() & CH376_ATTR_DIRECTORY) != 0;

            // FAT stores these two directory-navigation entries in every
            // subdirectory.  Navigation is provided by the screen's back
            // button, so they must not be presented as ordinary folders.
            if (entry.name == "." || entry.name == "..")
                continue;

            entries.push_back(entry);
        }

        return entries;
    }

    IStorageFile *openRead(const String &path) override
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

    IStorageFile *openWrite(const String &path) override
    {
        String dir, filename;
        splitPath(path, dir, filename);

        if (!navigateTo(dir))
            return nullptr;

        chip.setFileName(filename.c_str());
        chip.openFile(); // Creates the file if it doesn't exist

        return new UsbStorageFile(&chip, 0);
    }

    bool remove(const String &path) override
    {
        String dir, filename;
        splitPath(path, dir, filename);

        if (!navigateTo(dir))
            return false;

        chip.setFileName(filename.c_str());
        return chip.deleteFile();
    }

    bool rename(const String &oldPath, const String &newPath) override
    {
        // This library doesn't expose a direct rename command in its public API -
        // It would have to be implemented like copy+delete (read the entire old file,
        // write it with the new name, delete the old one). We'll leave that pending
        // until we confirm if there's a native method I've missed.
        return false;
    }

    bool mkdir(const String &path) override
    {
        // `cd()` with `flag=1` seems to create the folder if it doesn't exist, according to the official example -
        // I can't confirm the hardware version yet; the PCB isn't ready.
        return chip.cd(path.c_str(), 1) == ANSW_USB_INT_SUCCESS;
    }

    bool rmdir(const String &path) override
    {
        return false; // TODO: Implement with chip.deleteDir(path) after confirming its use
    }

    uint64_t totalBytes() override
    {
        return (uint64_t)chip.getTotalSectors() * 512ULL; // SECTORSIZE of the library = 512
    }

    const char *driverName() const override { return "USB"; }

private:
    Ch376msc chip;
    bool available = false;
    byte chipVersion = 0;
    static String formatFatName(const char *rawName)
    {
        String base;
        String extension;

        for (uint8_t i = 0; i < 8 && rawName[i] != '\0'; ++i)
        {
            if (rawName[i] != ' ')
                base += rawName[i];
        }

        for (uint8_t i = 8; i < 11 && rawName[i] != '\0'; ++i)
        {
            if (rawName[i] != ' ')
                extension += rawName[i];
        }

        return extension.length() ? base + "." + extension : base;
    }

    void splitPath(const String &fullPath, String &dir, String &filename)
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

    bool navigateTo(const String &path)
    {
        // The CH376 keeps a current working directory.  Always issue cd(),
        // including for '/', otherwise the UI can show '/' while the chip is
        // still enumerating the previously opened subdirectory.
        const char *target = (path.length() == 0) ? "/" : path.c_str();
        uint8_t result = chip.cd(target, 0); // 0 = do not create, only browse
        return (result == ANSW_USB_INT_SUCCESS || result == ANSW_ERR_OPEN_DIR);
    }
};
