#pragma once

#include <Arduino.h>
#include "../storage/istorage_driver.h"
#include "gcode_parser.h"

struct GCodeFileInfo
{
    static constexpr uint16_t MAX_PREVIEW_POINTS = 400;

    bool valid = false;
    float minX = 0, minY = 0, maxX = 0, maxY = 0;
    uint32_t totalLines = 0;
    uint16_t previewPointCount = 0;
    GCodePosition previewPoints[MAX_PREVIEW_POINTS];
};

class GCodeFileAnalyzer
{
public:
    // Iterates through the entire file ONCE (streaming, without loading it into memory).
    // This is the "expensive" operation - it's done only once when the file is selected, not on every frame.
    static GCodeFileInfo analyze(IStorageDriver& driver, const String& path, bool collectPreview = false);
    static uint32_t countLinesOnly(IStorageDriver& driver, const String& path); // Fast - Does NOT calculate bounding box, only counts lines
};
