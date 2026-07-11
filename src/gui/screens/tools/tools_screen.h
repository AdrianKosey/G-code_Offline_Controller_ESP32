#pragma once

#include "../../core/screen.h"
#include "../../theme.h"
#include "../../widgets/panel/panel_widget.h"
#include "../../widgets/label/label_widget.h"
#include "../../widgets/button/button_widget.h"
#include "../../../machine/grbl_controller.h"
#include "../../../../include/config.h"

enum class ToolMode
{
    Spindle,
    Laser
};

class ToolsScreen : public IScreen
{
public:
    ToolsScreen(GrblController& grblController);

private:
    GrblController& grbl;

    PanelWidget panel;

    LabelWidget modeLabel;
    ButtonWidget spindleModeButton;
    ButtonWidget laserModeButton;

    LabelWidget powerLabel;
    LabelWidget powerValueLabel;
    ButtonWidget powerMinusButton;
    ButtonWidget powerPlusButton;

    ButtonWidget directionButton; // Spindle (CW/CCW)
    ButtonWidget toggleButton;    // ON/OFF

    ToolMode mode = ToolMode::Spindle;
    float power = 0.0f;
    bool on = false;
    bool clockwise = true;

    void selectMode(ToolMode newMode);
    void updatePowerLabel();
    void applyOutput();
};