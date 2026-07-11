#include "framing_runner.h"

FramingRunner::FramingRunner(GrblController& grbl) : grbl(grbl) {}

void FramingRunner::start(float minX, float minY, float maxX, float maxY, float newFeedRate)
{
    feedRate = newFeedRate;

    // Complete perimeter: 4 corners and back to the starting point
    steps[0] = { minX, minY };
    steps[1] = { maxX, minY };
    steps[2] = { maxX, maxY };
    steps[3] = { minX, maxY };
    steps[4] = { minX, minY };

    stepCount = 5;
    currentStep = 0;
    waitingForOk = false;
    running = true;

    // The tool is turned off for safety before traversing the perimeter
    grbl.setSpindlePower(0.0f);
}

bool FramingRunner::isRunning() const { return running; }

void FramingRunner::sendNextStep()
{
    if (currentStep >= stepCount)
    {
        running = false;
        return;
    }

    const Point& p = steps[currentStep];

    String cmd = "G90 G1 X" + String(p.x, 3) + " Y" + String(p.y, 3) + " F" + String(feedRate, 0);

    grbl.sendLine(cmd);
    waitingForOk = true;
    currentStep++;
}

void FramingRunner::update()
{
    if (!running)
        return;

    if (waitingForOk)
    {
        if (!grbl.hasPendingOk())
            return;

        grbl.consumeOk();
        waitingForOk = false;
    }

    sendNextStep();

    if (currentStep == 1) // First transmission just made on this same call
        return;
}