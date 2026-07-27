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
    closeFile();

    state = JobState::Idle;
    currentLine = 0;
}

JobState GCodeJobRunner::getState() const { return state; }
uint32_t GCodeJobRunner::getCurrentLine() const { return currentLine; }
uint32_t GCodeJobRunner::getTotalLines() const { return totalLines; }

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