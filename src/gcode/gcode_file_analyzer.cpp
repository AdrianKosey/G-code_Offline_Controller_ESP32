#include "gcode_file_analyzer.h"

GCodeFileInfo GCodeFileAnalyzer::analyze(IStorageDriver& driver, const String& path)
{
    GCodeFileInfo info;

    IStorageFile* file = driver.openRead(path);
    if (!file || !file->isValid())
        return info;

    GCodeParser parser;
    bool first = true;

    while (file->available())
    {
        String line = file->readStringUntil('\n');
        GCodeCommand command = parser.parseLine(line);
        info.totalLines++;

        if (command.hasTarget)
        {
            if (first)
            {
                info.minX = info.maxX = command.target.x;
                info.minY = info.maxY = command.target.y;
                first = false;
            }
            else
            {
                info.minX = min(info.minX, command.target.x);
                info.maxX = max(info.maxX, command.target.x);
                info.minY = min(info.minY, command.target.y);
                info.maxY = max(info.maxY, command.target.y);
            }
        }
    }

    file->close();
    delete file;

    info.valid = !first;
    return info;
}

uint32_t GCodeFileAnalyzer::countLinesOnly(IStorageDriver& driver, const String& path)
{
    IStorageFile* file = driver.openRead(path);
    if (!file || !file->isValid())
        return 0;

    uint32_t lines = 0;
    uint8_t buffer[512];

    while (file->available())
    {
        size_t bytesRead = file->read(buffer, sizeof(buffer));

        for (size_t i = 0; i < bytesRead; i++)
        {
            if (buffer[i] == '\n')
                lines++;
        }
    }

    file->close();
    delete file;

    return lines;
}