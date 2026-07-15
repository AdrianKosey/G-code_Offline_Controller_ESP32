#pragma once

constexpr const char* CONTROL_NAME = "G-code Offline Controller";
constexpr const char* CONTROL_VERSION = "v0.11.6";

// Fallback values ​​for Grbl parameters.
// These are only used if the connected machine has not yet responded with "$$" (or in simulated mode
// before configuring anything else). As soon as GrblSettings has the actual value,
// that value always takes precedence - see GrblController::getSettings().get(number, fallback).
namespace GrblDefaults
{
    constexpr float S0  = 10;
    constexpr float S1  = 255;
    constexpr float S2  = 0;
    constexpr float S3  = 0;
    constexpr float S4  = 0;
    constexpr float S5  = 0;
    constexpr float S6  = 0;
    constexpr float S10 = 3;
    constexpr float S11 = 0.010f;
    constexpr float S12 = 0.002f;
    constexpr float S13 = 0;
    constexpr float S20 = 0;
    constexpr float S21 = 1;
    constexpr float S22 = 1;
    constexpr float S23 = 3;
    constexpr float S24 = 100.000f;
    constexpr float S25 = 1000.000f;
    constexpr float S26 = 250;
    constexpr float S27 = 2.000f;
    constexpr float S30 = 10000;
    constexpr float S31 = 0;
    constexpr float S32 = 0;
    constexpr float S100 = 400.000f;
    constexpr float S101 = 400.000f;
    constexpr float S102 = 400.000f;
    constexpr float S110 = 1500.000f;
    constexpr float S111 = 1500.000f;
    constexpr float S112 = 1500.000f;
    constexpr float S120 = 50.000f;
    constexpr float S121 = 50.000f;
    constexpr float S122 = 50.000f;
    constexpr float S130 = 300.000f;
    constexpr float S131 = 180.000f;
    constexpr float S132 = 60.000f;
}

constexpr float JOG_FEED_RATE = 1000.0f;      // mm/min
constexpr float FRAMING_FEED_RATE = 2000.0f;  // idem
constexpr bool FRAMING_ENABLED_DEFAULT = true;
constexpr float SAFE_Z_HEIGHT = 5.0f;
constexpr bool SCREEN_SLEEP_ENABLED_DEFAULT = false;
constexpr uint16_t SCREEN_SLEEP_MINUTES_DEFAULT = 5;

constexpr uint32_t BUZZER_FREQUENCY = 2500;
constexpr uint16_t BUZZER_DURATION_MS = 30;
constexpr bool BUZZER_ENABLED_DEFAULT = true;