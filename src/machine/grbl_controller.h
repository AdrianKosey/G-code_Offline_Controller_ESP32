#pragma once

#include <Arduino.h>
#include <map>
#include "../gcode/gcode_parser.h" // for simulation
#include "../../include/config.h"

enum class GrblConnectionState { Disconnected, Connected };

struct GrblSettings
{
    float values[133] = {0};
    bool present[133] = {false};

    float get(uint8_t index, float defaultValue = 0.0f) const
    {
        return present[index] ? values[index] : defaultValue;
    }
};

enum class GrblState
{
    Unknown, Idle, Run, Hold, Alarm, Door, Check, Home, Sleep
};

struct GrblStatus
{
    GrblState state = GrblState::Unknown;
    float x = 0, y = 0, z = 0;
    float feedRate = 0;
    float spindleSpeed = 0;
};

class GrblController
{
public:
    void begin(HardwareSerial& serialPort, unsigned long baud, int8_t rxPin, int8_t txPin);

    void update(); 

    void sendLine(const String& line);
    void requestStatus(); // '?'

    void feedHold();   // '!'
    void cycleStart(); // '~'
    void softReset();  // 0x18

    bool hasPendingOk() const;
    bool consumeOk();
    const GrblStatus& getStatus() const;

    void jog(char axis, float distance, float feedRate);
    void jogCancel();
    void home();
    void setWorkZero();
    void setSpindlePower(float percent, bool clockwise = true);
    void setWorkZeroAxis(char axis); 
    void probeZ(float maxDistance, float feedRate);

    void requestSettings(); // "$$"

    GrblConnectionState getConnectionState() const;
    bool isSettingsLoaded() const;
    const GrblSettings& getSettings() const;


    float getMaxSpindleSpeed() const { return settings.get(30, GrblDefaults::S30); }
    float getMaxTravelX() const { return settings.get(130, GrblDefaults::S130); }
    float getMaxTravelY() const { return settings.get(131, GrblDefaults::S131); }
    float getMaxTravelZ() const { return settings.get(132, GrblDefaults::S132); }
    bool isLaserMode() const { return settings.get(32, GrblDefaults::S32) > 0.5f; }

    void setSetting(uint8_t index, float value);

    // Simulation
    void beginSimulated();

private:
    HardwareSerial* serial = nullptr;
    String lineBuffer;
    GrblStatus status;

    bool okFlag = false;
    bool errorFlag = false;

    void processLine(const String& line);
    void parseStatusReport(const String& line);

    GrblConnectionState connectionState = GrblConnectionState::Disconnected;
    unsigned long lastResponseAt = 0;
    bool hasReceivedAny = false;
    static constexpr unsigned long CONNECTION_TIMEOUT_MS = 1000;

    GrblSettings settings;
    bool settingsLoaded = false;
    bool settingsRequested = false;

    void parseSettingLine(const String& line); // "$100=80.000"
    void markResponseReceived();

    // Simulation
    bool simulated = false;
    GCodeParser simParser;

    unsigned long simOkAt = 0;
    static constexpr unsigned long SIM_MOVE_DELAY_MS = 30;
};