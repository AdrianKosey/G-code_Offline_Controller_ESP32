#pragma once

#include <Arduino.h>
#include "../gcode/gcode_parser.h" // for simulation
#include "../../include/config.h"
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

    // Simulation
    bool simulated = false;
    GCodeParser simParser;

    unsigned long simOkAt = 0;
    static constexpr unsigned long SIM_MOVE_DELAY_MS = 30;
};