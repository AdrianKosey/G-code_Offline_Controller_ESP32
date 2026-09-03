#include "gcode_file_analyzer.h"

namespace
{
    // Reads storage in blocks so SD and USB analysis do not issue one I/O
    // request for every character in a G-code line.
    class BufferedLineReader
    {
    public:
        explicit BufferedLineReader(IStorageFile& file) : file(file) {}

        bool readLine(String& line)
        {
            line = "";
            bool receivedAny = false;

            while (true)
            {
                if (offset >= length)
                {
                    length = file.read(buffer, sizeof(buffer));
                    offset = 0;

                    if (length == 0)
                        return receivedAny;
                }

                char character = (char)buffer[offset++];
                receivedAny = true;

                if (character == '\n')
                    return true;

                if (character != '\r')
                    line += character;
            }
        }

    private:
        IStorageFile& file;
        uint8_t buffer[256];
        size_t offset = 0;
        size_t length = 0;
    };

    void addPreviewPoint(GCodeFileInfo& info, const GCodePosition& point,
                         uint32_t targetIndex, uint16_t& sampleStride)
    {
        if (info.previewPointCount == GCodeFileInfo::MAX_PREVIEW_POINTS)
        {
            // Retain every other point and double the sampling interval. This
            // keeps representative points in drawing order without a second
            // pass through the G-code file.
            for (uint16_t i = 0; i < GCodeFileInfo::MAX_PREVIEW_POINTS / 2; ++i)
                info.previewPoints[i] = info.previewPoints[i * 2];

            info.previewPointCount /= 2;
            sampleStride *= 2;
        }

        if ((targetIndex % sampleStride) == 0)
            info.previewPoints[info.previewPointCount++] = point;
    }
}

GCodeFileInfo GCodeFileAnalyzer::analyze(IStorageDriver& driver, const String& path, bool collectPreview)
{
    GCodeFileInfo info;

    IStorageFile* file = driver.openRead(path);
    if (!file || !file->isValid())
        return info;

    GCodeParser parser;
    BufferedLineReader reader(*file);
    bool first = true;
    uint32_t targetIndex = 0;
    uint16_t sampleStride = 1;

    String line;
    while (reader.readLine(line))
    {
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

            if (collectPreview)
                addPreviewPoint(info, command.target, targetIndex, sampleStride);

            targetIndex++;
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
