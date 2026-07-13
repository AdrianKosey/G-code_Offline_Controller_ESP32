#include "grbl_setting_meta.h"

namespace
{
    const StringId STATUS_REPORT_OPTION_IDS[] = { StringId::StatusMPos, StringId::StatusWPos };
}

const GrblSettingDef GRBL_SETTINGS_TABLE[] = {
    { 0,   StringId::Setting0_StepPulse,     GrblSettingType::Numeric, nullptr, 0, 1, 255, 1, StringId::GRBL_Value_Type1 },
    { 1,   StringId::Setting1_StepDelay,     GrblSettingType::Numeric, nullptr, 0, 0, 255, 1, StringId::GRBL_Value_Type2 },
    { 2,   StringId::Setting2_StepInvert,    GrblSettingType::Numeric, nullptr, 0, 0, 7, 1, StringId::GRBL_Value_Type3 },
    { 3,   StringId::Setting3_DirInvert,     GrblSettingType::Numeric, nullptr, 0, 0, 7, 1, StringId::GRBL_Value_Type3 },
    { 4,   StringId::Setting4_EnableInvert,  GrblSettingType::Toggle },
    { 5,   StringId::Setting5_LimitInvert,   GrblSettingType::Toggle },
    { 6,   StringId::Setting6_ProbeInvert,   GrblSettingType::Toggle },
    { 10,  StringId::Setting10_StatusReport, GrblSettingType::Enum, STATUS_REPORT_OPTION_IDS, 2 },
    { 11,  StringId::Setting11_JunctionDev,  GrblSettingType::Numeric, nullptr, 0, 0.001f, 1.0f, 0.001f, StringId::GRBL_Value_Type4 },
    { 12,  StringId::Setting12_ArcTolerance, GrblSettingType::Numeric, nullptr, 0, 0.001f, 1.0f, 0.001f, StringId::GRBL_Value_Type4 },
    { 13,  StringId::Setting13_ReportInches, GrblSettingType::Toggle },
    { 20,  StringId::Setting20_SoftLimits,   GrblSettingType::Toggle },
    { 21,  StringId::Setting21_HardLimits,   GrblSettingType::Toggle },
    { 22,  StringId::Setting22_HomingCycle,  GrblSettingType::Toggle },
    { 23,  StringId::Setting23_HomingDir,    GrblSettingType::Numeric, nullptr, 0, 0, 7, 1, StringId::GRBL_Value_Type3 },
    { 24,  StringId::Setting24_HomingFeed,   GrblSettingType::Numeric, nullptr, 0, 1, 1000, 1, StringId::GRBL_Value_Type5 },
    { 25,  StringId::Setting25_HomingSeek,   GrblSettingType::Numeric, nullptr, 0, 1, 5000, 1, StringId::GRBL_Value_Type5 },
    { 26,  StringId::Setting26_HomingDebounce, GrblSettingType::Numeric, nullptr, 0, 0, 255, 1, StringId::GRBL_Value_Type2 },
    { 27,  StringId::Setting27_HomingPulloff, GrblSettingType::Numeric, nullptr, 0, 0, 10, 0.1f, StringId::GRBL_Value_Type4 },
    { 30,  StringId::Setting30_MaxSpindle,   GrblSettingType::Numeric, nullptr, 0, 0, 30000, 100, StringId::GRBL_Value_Type6 },
    { 31,  StringId::Setting31_MinSpindle,   GrblSettingType::Numeric, nullptr, 0, 0, 30000, 100, StringId::GRBL_Value_Type6 },
    { 32,  StringId::Setting32_LaserMode,    GrblSettingType::Toggle },
    { 100, StringId::Setting100_StepsX,      GrblSettingType::Numeric, nullptr, 0, 1, 5000, 1, StringId::GRBL_Value_Type7 },
    { 101, StringId::Setting101_StepsY,      GrblSettingType::Numeric, nullptr, 0, 1, 5000, 1, StringId::GRBL_Value_Type7 },
    { 102, StringId::Setting102_StepsZ,      GrblSettingType::Numeric, nullptr, 0, 1, 5000, 1, StringId::GRBL_Value_Type7 },
    { 110, StringId::Setting110_MaxRateX,    GrblSettingType::Numeric, nullptr, 0, 1, 10000, 10, StringId::GRBL_Value_Type5 },
    { 111, StringId::Setting111_MaxRateY,    GrblSettingType::Numeric, nullptr, 0, 1, 10000, 10, StringId::GRBL_Value_Type5 },
    { 112, StringId::Setting112_MaxRateZ,    GrblSettingType::Numeric, nullptr, 0, 1, 10000, 10, StringId::GRBL_Value_Type5 },
    { 120, StringId::Setting120_AccelX,      GrblSettingType::Numeric, nullptr, 0, 1, 1000, 1, StringId::GRBL_Value_Type8 },
    { 121, StringId::Setting121_AccelY,      GrblSettingType::Numeric, nullptr, 0, 1, 1000, 1, StringId::GRBL_Value_Type8 },
    { 122, StringId::Setting122_AccelZ,      GrblSettingType::Numeric, nullptr, 0, 1, 1000, 1, StringId::GRBL_Value_Type8 },
    { 130, StringId::Setting130_TravelX,     GrblSettingType::Numeric, nullptr, 0, 1, 5000, 1, StringId::GRBL_Value_Type4 },
    { 131, StringId::Setting131_TravelY,     GrblSettingType::Numeric, nullptr, 0, 1, 5000, 1, StringId::GRBL_Value_Type4 },
    { 132, StringId::Setting132_TravelZ,     GrblSettingType::Numeric, nullptr, 0, 1, 5000, 1, StringId::GRBL_Value_Type4 },
};

const uint8_t GRBL_SETTINGS_TABLE_COUNT = sizeof(GRBL_SETTINGS_TABLE) / sizeof(GrblSettingDef);

const GrblSettingDef* findGrblSettingDef(uint8_t index)
{
    for (uint8_t i = 0; i < GRBL_SETTINGS_TABLE_COUNT; i++)
    {
        if (GRBL_SETTINGS_TABLE[i].index == index)
            return &GRBL_SETTINGS_TABLE[i];
    }

    return nullptr;
}