#include "jog_screen.h"

static constexpr int16_t CONTENT_X = 66;

JogScreen::JogScreen(GrblController& grblController)
    : grbl(grblController),

      stepLabel(Rect{CONTENT_X + 8, 36, 100, 14}, "Paso (mm)", Theme::TextSecondary, 1, Theme::Panel, false),
      jogPanel(Rect{CONTENT_X + 2, 34, 250, 200}, Theme::Panel, 10),
      step01Button(Rect{CONTENT_X + 8, 52, 46, 26}, "0.1"),
      step1Button(Rect{CONTENT_X + 58, 52, 46, 26}, "1"),
      step10Button(Rect{CONTENT_X + 108, 52, 46, 26}, "10"),
      probeZButton(Rect{CONTENT_X + 158, 52, 86, 26}, "PROBE Z"),

      yPlusButton(Rect{CONTENT_X + 90, 88, 40, 32}, "Y+"),
      xMinusButton(Rect{CONTENT_X + 40, 124, 40, 32}, "X-"),
      xPlusButton(Rect{CONTENT_X + 140, 124, 40, 32}, "X+"),
      yMinusButton(Rect{CONTENT_X + 90, 160, 40, 32}, "Y-"),

      zPlusButton(Rect{CONTENT_X + 200, 88, 44, 32}, "Z+"),
      zMinusButton(Rect{CONTENT_X + 200, 160, 44, 32}, "Z-"),

      homeButton(Rect{CONTENT_X + 8, 200, 56, 28}, "HOME"),
      setXZeroButton(Rect{CONTENT_X + 68, 200, 56, 28}, "SET X0"),
      setYZeroButton(Rect{CONTENT_X + 128, 200, 56, 28}, "SET Y0"),
      setZZeroButton(Rect{CONTENT_X + 188, 200, 56, 28}, "SET Z0")

{
    step1Button.setSelected(true);

    step01Button.setOnPress([this]() { selectStep(0.1f, &step01Button); });
    step1Button.setOnPress([this]()  { selectStep(1.0f, &step1Button); });
    step10Button.setOnPress([this]() { selectStep(10.0f, &step10Button); });

    xPlusButton.setOnPress([this]()  { grbl.jog('X', jogStep, JOG_FEED_RATE); });
    xMinusButton.setOnPress([this]() { grbl.jog('X', -jogStep, JOG_FEED_RATE); });
    yPlusButton.setOnPress([this]()  { grbl.jog('Y', jogStep, JOG_FEED_RATE); });
    yMinusButton.setOnPress([this]() { grbl.jog('Y', -jogStep, JOG_FEED_RATE); });
    zPlusButton.setOnPress([this]()  { grbl.jog('Z', jogStep, JOG_FEED_RATE); });
    zMinusButton.setOnPress([this]() { grbl.jog('Z', -jogStep, JOG_FEED_RATE); });

    homeButton.setOnPress([this]() { grbl.home(); });

    setXZeroButton.setOnPress([this]() { grbl.setWorkZeroAxis('X'); });
    setYZeroButton.setOnPress([this]() { grbl.setWorkZeroAxis('Y'); });
    setZZeroButton.setOnPress([this]() { grbl.setWorkZeroAxis('Z'); });

    probeZButton.setOnPress([this]() {
        grbl.probeZ(15.0f, 100.0f);
        waitingForProbeZero = true;
    });

    widgets = {
        &jogPanel, &stepLabel, &step01Button, &step1Button, &step10Button,
        &yPlusButton, &xMinusButton, &xPlusButton, &yMinusButton,
        &zPlusButton, &zMinusButton,
        &homeButton, &setXZeroButton, &setYZeroButton, &setZZeroButton,
        &probeZButton
    };
}

void JogScreen::update()
{
    IScreen::update();

    if (waitingForProbeZero && grbl.consumeOk())
    {
        grbl.setWorkZeroAxis('Z');
        waitingForProbeZero = false;
    }
}

void JogScreen::selectStep(float step, ButtonWidget* selectedButton)
{
    jogStep = step;

    step01Button.setSelected(selectedButton == &step01Button);
    step1Button.setSelected(selectedButton == &step1Button);
    step10Button.setSelected(selectedButton == &step10Button);
}