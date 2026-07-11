#pragma once

#include <Arduino.h>
#include <SD.h>
#include "../machine/grbl_controller.h"

enum class JobState
{
    Idle, Loaded, Running, Paused, Completed, Error
};

class GCodeJobRunner
{
public:
    GCodeJobRunner(GrblController& grbl);

    bool load(const String& path, uint32_t totalLines);
    void start();
    void pause();
    void resume();
    void stop();

    void update(); // no bloqueante

    JobState getState() const;
    uint32_t getCurrentLine() const;
    uint32_t getTotalLines() const;

private:
    GrblController& grbl;

    File file;
    JobState state = JobState::Idle;

    uint32_t currentLine = 0;
    uint32_t totalLines = 0;

    bool waitingForOk = false;

    unsigned long lastStatusRequest = 0;
    static constexpr unsigned long STATUS_INTERVAL_MS = 200;

    void sendNextLine();
};