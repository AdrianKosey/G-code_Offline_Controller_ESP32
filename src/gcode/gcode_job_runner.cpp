#include "gcode_job_runner.h"

GCodeJobRunner::GCodeJobRunner(GrblController& grbl) : grbl(grbl) {}

bool GCodeJobRunner::load(const String& path, uint32_t newTotalLines)
{
    if (file) file.close();

    file = SD.open(path);
    if (!file || file.isDirectory())
    {
        state = JobState::Error;
        return false;
    }

    totalLines = newTotalLines;
    currentLine = 0;
    waitingForOk = false;
    state = JobState::Loaded;
    return true;
}

void GCodeJobRunner::start()
{
    if (state != JobState::Loaded && state != JobState::Completed)
        return;

    file.seek(0);
    currentLine = 0;
    waitingForOk = false;
    state = JobState::Running;
}

void GCodeJobRunner::pause()
{
    if (state != JobState::Running) return;
    grbl.feedHold();
    state = JobState::Paused;
}

void GCodeJobRunner::resume()
{
    if (state != JobState::Paused) return;
    grbl.cycleStart();
    state = JobState::Running;
}

void GCodeJobRunner::stop()
{
    if (state != JobState::Running && state != JobState::Paused) return;

    grbl.softReset();
    if (file) file.close();

    state = JobState::Idle;
    currentLine = 0;
}

JobState GCodeJobRunner::getState() const { return state; }
uint32_t GCodeJobRunner::getCurrentLine() const { return currentLine; }
uint32_t GCodeJobRunner::getTotalLines() const { return totalLines; }

void GCodeJobRunner::sendNextLine()
{
    while (file.available())
    {
        String line = file.readStringUntil('\n');
        line.trim();
        currentLine++;

        if (line.length() == 0 || line.startsWith(";") || line.startsWith("("))
            continue; 

        grbl.sendLine(line);
        waitingForOk = true;
        return;
    }

    state = JobState::Completed;
    file.close();
}

void GCodeJobRunner::update()
{
    grbl.update();

    unsigned long now = millis();
    if (now - lastStatusRequest > STATUS_INTERVAL_MS)
    {
        grbl.requestStatus();
        lastStatusRequest = now;
    }

    if (state != JobState::Running)
        return;

    if (waitingForOk)
    {
        if (!grbl.hasPendingOk())
            return; 

        waitingForOk = false;
    }

    sendNextLine();
}