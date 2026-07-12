#pragma once

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