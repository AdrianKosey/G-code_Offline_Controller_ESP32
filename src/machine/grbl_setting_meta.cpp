#include "grbl_setting_meta.h"

namespace
{
    const char* STEP_PULSE_OPTIONS[] = {}; // numeric, without enum

    const char* STATUS_REPORT_OPTIONS[] = { "Posicion maquina (MPos)", "Posicion de trabajo (WPos)" };
}

const GrblSettingDef GRBL_SETTINGS_TABLE[] = {
    { 0,  "Duracion pulso step",       GrblSettingType::Numeric, nullptr, 0, 1, 255, 1, "us" },
    { 1,  "Retardo desactivar steppers", GrblSettingType::Numeric, nullptr, 0, 0, 255, 1, "ms" },
    { 2,  "Invertir puerto step",      GrblSettingType::Numeric, nullptr, 0, 0, 7, 1, "mascara" },
    { 3,  "Invertir puerto direccion", GrblSettingType::Numeric, nullptr, 0, 0, 7, 1, "mascara" },
    { 4,  "Invertir habilitar steppers", GrblSettingType::Toggle },
    { 5,  "Invertir limit switches",   GrblSettingType::Toggle },
    { 6,  "Invertir probe",            GrblSettingType::Toggle },
    { 10, "Opciones reporte estado",   GrblSettingType::Enum, STATUS_REPORT_OPTIONS, 2 },
    { 11, "Desviacion de union",       GrblSettingType::Numeric, nullptr, 0, 0.001f, 1.0f, 0.001f, "mm" },
    { 12, "Tolerancia de arco",        GrblSettingType::Numeric, nullptr, 0, 0.001f, 1.0f, 0.001f, "mm" },
    { 13, "Reporte en pulgadas",       GrblSettingType::Toggle },
    { 20, "Limites de software",       GrblSettingType::Toggle },
    { 21, "Limites de hardware",       GrblSettingType::Toggle },
    { 22, "Ciclo de homing",           GrblSettingType::Toggle },
    { 23, "Direccion de homing",       GrblSettingType::Numeric, nullptr, 0, 0, 7, 1, "mascara" },
    { 24, "Feed de homing",            GrblSettingType::Numeric, nullptr, 0, 1, 1000, 1, "mm/min" },
    { 25, "Feed de busqueda homing",   GrblSettingType::Numeric, nullptr, 0, 1, 5000, 1, "mm/min" },
    { 26, "Debounce homing",           GrblSettingType::Numeric, nullptr, 0, 0, 255, 1, "ms" },
    { 27, "Retraccion homing",         GrblSettingType::Numeric, nullptr, 0, 0, 10, 0.1f, "mm" },
    { 30, "Velocidad max spindle",     GrblSettingType::Numeric, nullptr, 0, 0, 30000, 100, "RPM" },
    { 31, "Velocidad min spindle",     GrblSettingType::Numeric, nullptr, 0, 0, 30000, 100, "RPM" },
    { 32, "Modo laser",                GrblSettingType::Toggle },
    { 100, "Pasos/mm eje X",           GrblSettingType::Numeric, nullptr, 0, 1, 5000, 1, "pasos/mm" },
    { 101, "Pasos/mm eje Y",           GrblSettingType::Numeric, nullptr, 0, 1, 5000, 1, "pasos/mm" },
    { 102, "Pasos/mm eje Z",           GrblSettingType::Numeric, nullptr, 0, 1, 5000, 1, "pasos/mm" },
    { 110, "Velocidad max X",          GrblSettingType::Numeric, nullptr, 0, 1, 10000, 10, "mm/min" },
    { 111, "Velocidad max Y",          GrblSettingType::Numeric, nullptr, 0, 1, 10000, 10, "mm/min" },
    { 112, "Velocidad max Z",          GrblSettingType::Numeric, nullptr, 0, 1, 10000, 10, "mm/min" },
    { 120, "Aceleracion X",            GrblSettingType::Numeric, nullptr, 0, 1, 1000, 1, "mm/s2" },
    { 121, "Aceleracion Y",            GrblSettingType::Numeric, nullptr, 0, 1, 1000, 1, "mm/s2" },
    { 122, "Aceleracion Z",            GrblSettingType::Numeric, nullptr, 0, 1, 1000, 1, "mm/s2" },
    { 130, "Recorrido max X",          GrblSettingType::Numeric, nullptr, 0, 1, 5000, 1, "mm" },
    { 131, "Recorrido max Y",          GrblSettingType::Numeric, nullptr, 0, 1, 5000, 1, "mm" },
    { 132, "Recorrido max Z",          GrblSettingType::Numeric, nullptr, 0, 1, 5000, 1, "mm" },
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