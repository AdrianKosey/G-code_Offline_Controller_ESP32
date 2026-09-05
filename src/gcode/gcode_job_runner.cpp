#include "gcode_job_runner.h"

GCodeJobRunner::GCodeJobRunner(GrblController& grbl) : grbl(grbl) {}

void GCodeJobRunner::closeFile()
{
    if (file)
    {
        file->close();
        delete file;
        file = nullptr;
    }
}

bool GCodeJobRunner::load(IStorageDriver& storageDriver, const String& path, uint32_t newTotalLines)
{
    closeFile();

    driver = &storageDriver;
    currentPath = path;

    file = driver->openRead(path);
    if (!file || !file->isValid())
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

    closeFile();
    file = driver->openRead(currentPath);

    currentLine = 0;
    waitingForOk = false;
    parser.reset();
    state = JobState::Running;

    jobStartedAt = millis();
    pausedAccumulated = 0;
    timeFrozen = false;
}

void GCodeJobRunner::pause()
{
    if (state != JobState::Running) return;
    grbl.feedHold();
    state = JobState::Paused;

    pauseStartedAt = millis();
}

void GCodeJobRunner::resume()
{
    if (state != JobState::Paused) return;
    grbl.cycleStart();
    state = JobState::Running;

    pausedAccumulated += millis() - pauseStartedAt;
}

void GCodeJobRunner::stop()
{
    if (state != JobState::Running && state != JobState::Paused) return;

    grbl.softReset();
    closeFile();

    state = JobState::Idle;
    currentLine = 0;

    frozenElapsedSeconds = getElapsedSeconds();
    timeFrozen = true;
}

JobState GCodeJobRunner::getState() const { return state; }
uint32_t GCodeJobRunner::getCurrentLine() const { return currentLine; }
uint32_t GCodeJobRunner::getTotalLines() const { return totalLines; }

unsigned long GCodeJobRunner::getElapsedSeconds() const
{
    if (timeFrozen)
        return frozenElapsedSeconds;

    if (jobStartedAt == 0)
        return 0;

    unsigned long now = (state == JobState::Paused) ? pauseStartedAt : millis();
    unsigned long elapsed = now - jobStartedAt - pausedAccumulated;

    return elapsed / 1000;
}

void GCodeJobRunner::sendNextLine()
{
    while (file && file->available())
    {
        String line = file->readStringUntil('\n');
        line.trim();
        currentLine++;

        GCodeCommand command = parser.parseLine(line);

        if (line.length() == 0 || line.startsWith(";") || line.startsWith("("))
            continue;

        grbl.sendLine(line);
        waitingForOk = true;
        return;
    }

    state = JobState::Completed;
    closeFile();

    frozenElapsedSeconds = getElapsedSeconds();
    timeFrozen = true;
}

void GCodeJobRunner::resumeFrom(IStorageDriver& storageDriver, const String& path, uint32_t fromLine, uint32_t totalLinesParam)
{
    closeFile();

    driver = &storageDriver;
    currentPath = path;

    file = driver->openRead(path);
    if (!file || !file->isValid())
    {
        state = JobState::Error;
        return;
    }

    totalLines = totalLinesParam;
    currentLine = 0;
    waitingForOk = false;

    parser.reset();

    while (file->available() && currentLine < fromLine)
    {
        String line = file->readStringUntil('\n');
        line.trim();
        parser.parseLine(line);
        currentLine++;
    }

    state = JobState::Running;

    state = JobState::Running;
    jobStartedAt = millis();
    pausedAccumulated = 0;
    timeFrozen = false;
}

void GCodeJobRunner::update()
{
    grbl.update();

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