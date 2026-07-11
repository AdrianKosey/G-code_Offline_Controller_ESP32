#include "gcode_file_analyzer.h"

GCodeFileInfo GCodeFileAnalyzer::analyze(const String& path)
{
    GCodeFileInfo info;

    File file = SD.open(path);
    if (!file || file.isDirectory())
        return info;

    GCodeParser parser;
    bool first = true;

    while (file.available())
    {
        String line = file.readStringUntil('\n');
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

    file.close();
    info.valid = !first;

    return info;
}