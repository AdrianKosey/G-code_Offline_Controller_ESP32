#pragma once
#include <Arduino.h>

class IStorageFile
{
public:
    virtual ~IStorageFile() = default;

    virtual bool isValid() const = 0;
    virtual bool available() = 0;
    virtual String readStringUntil(char terminator) = 0;
    virtual size_t write(const uint8_t* buffer, size_t size) = 0;
    virtual size_t read(uint8_t* buffer, size_t size) = 0;
    virtual uint32_t size() const = 0;
    virtual void close() = 0;
};