#pragma once

#include <Arduino.h>
#include "grbl_controller.h"

class FramingRunner
{
public:
    FramingRunner(GrblController& grbl);

    void start(float minX, float minY, float maxX, float maxY, float feedRate);
    void update();

    bool isRunning() const;

private:
    GrblController& grbl;

    struct Point { float x, y; };
    static constexpr uint8_t MAX_STEPS = 5;

    Point steps[MAX_STEPS];
    uint8_t stepCount = 0;
    uint8_t currentStep = 0;

    bool running = false;
    bool waitingForOk = false;
    float feedRate = 0;

    void sendNextStep();
};