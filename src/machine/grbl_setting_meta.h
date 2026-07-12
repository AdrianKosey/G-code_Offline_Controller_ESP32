#pragma once

#include <Arduino.h>

enum class GrblSettingType
{
    Toggle,   // 0/1
    Enum,     // several discrete values ​​with meaning (e.g., 0/1/2/3)
    Numeric   // free value (speeds, distances, etc.)
};

struct GrblSettingDef
{
    uint8_t index;
    const char* name;
    GrblSettingType type;

    const char** enumLabels = nullptr; // Enum only
    uint8_t enumCount = 0;

    float min = 0;
    float max = 100000;
    float step = 1;
    const char* unit = ""; // "mm", "mm/min", "RPM", etc
};

// Table of known parameters - expand this list with any that are missing
extern const GrblSettingDef GRBL_SETTINGS_TABLE[];
extern const uint8_t GRBL_SETTINGS_TABLE_COUNT;

const GrblSettingDef* findGrblSettingDef(uint8_t index);