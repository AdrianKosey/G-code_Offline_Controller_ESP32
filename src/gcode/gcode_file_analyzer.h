#pragma once

#include <Arduino.h>
#include <SD.h>
#include "gcode_parser.h"

struct GCodeFileInfo
{
    bool valid = false;
    float minX = 0, minY = 0, maxX = 0, maxY = 0;
    uint32_t totalLines = 0;
};

class GCodeFileAnalyzer
{
public:
    // Iterates through the entire file ONCE (streaming, without loading it into memory).
    // This is the "expensive" operation - it's done only once when the file is selected, not on every frame.
    static GCodeFileInfo analyze(const String& path);
};