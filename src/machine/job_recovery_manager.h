#pragma once

#include <Arduino.h>
#include <SD.h>
#include "../gcode/gcode_state.h"

struct RecoverySnapshot
{
    bool valid = false;
    char path[80] = {0};
    uint32_t line = 0;
    uint32_t totalLines = 0;

    float x = 0, y = 0, z = 0;
    bool absoluteMode = true;
    bool metric = true;
    float feedRate = 0;
    float spindleSpeed = 0;
    uint8_t spindleState = 0;
    uint8_t plane = 17;
    uint8_t coordSystem = 54;
    uint8_t toolNumber = 0;
};

class JobRecoveryManager
{
public:
    void begin();

    void startJob(const String& path, uint32_t totalLines);
    void updateProgress(uint32_t line, const GCodeState& state, uint8_t spindleStateValue);
    void clear();

    bool hasPendingRecovery() const;
    const RecoverySnapshot& getSnapshot() const;

private:
    static constexpr const char* RECOVERY_PATH = "/recovery.dat";

    RecoverySnapshot snapshot;

    unsigned long lastSaveAt = 0;
    uint32_t lastSavedLine = 0;

    static constexpr unsigned long SAVE_INTERVAL_MS = 4000;
    static constexpr uint32_t SAVE_LINE_INTERVAL = 25;

    void persist();
};