#include "jog_screen.h"
#include "../../../i18n/translations.h"
static constexpr int16_t CONTENT_X = 60;

JogScreen::JogScreen(GrblController& grblController, AppSettingsManager& appSettingsManager)
    : grbl(grblController),
      appSettings(appSettingsManager),

      stepLabel(Rect{CONTENT_X + 8, 36, 100, 14}, tr(StringId::Jog_StepLabel), Theme::TextSecondary, 1, Theme::Background, false),

      step01Button(Rect{CONTENT_X + 8, 52, 46, 26}, "0.1"),
      step1Button(Rect{CONTENT_X + 58, 52, 46, 26}, "1"),
      step10Button(Rect{CONTENT_X + 108, 52, 46, 26}, "10"),

      yPlusButton(Rect{CONTENT_X + 90, 88, 40, 32}, tr(StringId::Jog_YUp)),
      xMinusButton(Rect{CONTENT_X + 40, 124, 40, 32}, tr(StringId::Jog_XDown)),
      xPlusButton(Rect{CONTENT_X + 140, 124, 40, 32}, tr(StringId::Jog_XUp)),
      yMinusButton(Rect{CONTENT_X + 90, 160, 40, 32}, tr(StringId::Jog_YDown)),

      zPlusButton(Rect{CONTENT_X + 200, 88, 44, 32}, tr(StringId::Jog_ZUp)),
      zMinusButton(Rect{CONTENT_X + 200, 160, 44, 32}, tr(StringId::Jog_ZDown)),

      homeButton(Rect{CONTENT_X + 8, 200, 56, 28}, tr(StringId::Jog_Home)),
      setXZeroButton(Rect{CONTENT_X + 68, 200, 56, 28}, tr(StringId::Jog_SetX0)),
      setYZeroButton(Rect{CONTENT_X + 128, 200, 56, 28}, tr(StringId::Jog_SetY0)),
      setZZeroButton(Rect{CONTENT_X + 188, 200, 56, 28}, tr(StringId::Jog_SetZ0)),

      probeZButton(Rect{CONTENT_X + 170, 52, 74, 26}, tr(StringId::Jog_ProbeZ))
{
    step1Button.setSelected(true);

    step01Button.setOnPress([this]() { selectStep(0.1f, &step01Button); });
    step1Button.setOnPress([this]()  { selectStep(1.0f, &step1Button); });
    step10Button.setOnPress([this]() { selectStep(10.0f, &step10Button); });

    xPlusButton.setOnPress([this]()  { grbl.jog('X', jogStep, appSettings.getJogFeedRate()); });
    xMinusButton.setOnPress([this]() { grbl.jog('X', -jogStep, appSettings.getJogFeedRate()); });
    yPlusButton.setOnPress([this]()  { grbl.jog('Y', jogStep, appSettings.getJogFeedRate()); });
    yMinusButton.setOnPress([this]() { grbl.jog('Y', -jogStep, appSettings.getJogFeedRate()); });
    zPlusButton.setOnPress([this]()  { grbl.jog('Z', jogStep, appSettings.getJogFeedRate()); });
    zMinusButton.setOnPress([this]() { grbl.jog('Z', -jogStep, appSettings.getJogFeedRate()); });

    homeButton.setOnPress([this]() { grbl.home(); });

    setXZeroButton.setOnPress([this]() { grbl.setWorkZeroAxis('X'); });
    setYZeroButton.setOnPress([this]() { grbl.setWorkZeroAxis('Y'); });
    setZZeroButton.setOnPress([this]() { grbl.setWorkZeroAxis('Z'); });

    probeZButton.setOnPress([this]() {
        grbl.probeZ(15.0f, 100.0f);
        waitingForProbeZero = true;
    });

    widgets = {
        &stepLabel, &step01Button, &step1Button, &step10Button,
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