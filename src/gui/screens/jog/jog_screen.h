#pragma once

#include "../../core/screen.h"
#include "../../theme.h"
#include "../../widgets/label/label_widget.h"
#include "../../widgets/button/button_widget.h"
#include "../../../machine/grbl_controller.h"
#include "../../../app/app_settings_manager.h"

class JogScreen : public IScreen
{
public:
    JogScreen(GrblController& grblController, AppSettingsManager& appSettingsManager);

    void update() override;

private:
    GrblController& grbl;
    AppSettingsManager& appSettings;

    LabelWidget stepLabel;
    ButtonWidget step01Button, step1Button, step10Button;

    ButtonWidget xPlusButton, xMinusButton;
    ButtonWidget yPlusButton, yMinusButton;
    ButtonWidget zPlusButton, zMinusButton;

    ButtonWidget homeButton;
    ButtonWidget setXZeroButton, setYZeroButton, setZZeroButton;
    ButtonWidget probeZButton;

    float jogStep = 1.0f;
    bool waitingForProbeZero = false;

    void selectStep(float step, ButtonWidget* selectedButton);
};