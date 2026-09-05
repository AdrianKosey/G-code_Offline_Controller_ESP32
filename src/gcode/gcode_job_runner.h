#pragma once

#include <Arduino.h>
#include "../machine/grbl_controller.h"
#include "../storage/istorage_driver.h"
#include "gcode_parser.h"

enum class JobState { Idle, Loaded, Running, Paused, Completed, Error };

class GCodeJobRunner
{
public:
    GCodeJobRunner(GrblController& grbl);

    bool load(IStorageDriver& driver, const String& path, uint32_t totalLines);
    void resumeFrom(IStorageDriver& driver, const String& path, uint32_t fromLine, uint32_t totalLines);

    void start();
    void pause();
    void resume();
    void stop();

    void update();

    unsigned long getElapsedSeconds() const;

    JobState getState() const;
    uint32_t getCurrentLine() const;
    uint32_t getTotalLines() const;

    const GCodeState& getParserState() const { return parser.getState(); }

private:
    GrblController& grbl;

    IStorageDriver* driver = nullptr;
    IStorageFile* file = nullptr;
    String currentPath;

    JobState state = JobState::Idle;

    uint32_t currentLine = 0;
    uint32_t totalLines = 0;

    bool waitingForOk = false;

    GCodeParser parser;

    void sendNextLine();
    void closeFile();

    unsigned long jobStartedAt = 0;
    unsigned long pausedAccumulated = 0;
    unsigned long pauseStartedAt = 0;
    unsigned long frozenElapsedSeconds = 0;
    bool timeFrozen = false;
};