#include "tools_screen.h"

static constexpr int16_t CONTENT_X = 60;

ToolsScreen::ToolsScreen(GrblController& grblController)
    : grbl(grblController),

      panel(Rect{CONTENT_X + 8, 34, 200, 160}, Theme::Panel, 10),

      modeLabel(Rect{CONTENT_X + 18, 42, 100, 14}, "Tipo de herramienta", Theme::TextSecondary, 1, Theme::Panel, false),
      spindleModeButton(Rect{CONTENT_X + 18, 58, 85, 30}, "Spindle"),
      laserModeButton(Rect{CONTENT_X + 111, 58, 85, 30}, "Laser"),

      powerLabel(Rect{CONTENT_X + 18, 98, 100, 14}, "Potencia", Theme::TextSecondary, 1, Theme::Panel, false),
      powerValueLabel(Rect{CONTENT_X + 18, 112, 100, 24}, "0 RPM", Theme::Text, 2, Theme::Panel, false),

      powerMinusButton(Rect{CONTENT_X + 120, 108, 36, 32}, "-10"),
      powerPlusButton(Rect{CONTENT_X + 162, 108, 36, 32}, "+10"),

      directionButton(Rect{CONTENT_X + 18, 148, 85, 32}, "CW"),
      toggleButton(Rect{CONTENT_X + 111, 148, 85, 32}, "ON")
{
    spindleModeButton.setSelected(true);

    spindleModeButton.setOnPress([this]() { selectMode(ToolMode::Spindle); });
    laserModeButton.setOnPress([this]()   { selectMode(ToolMode::Laser); });

    powerMinusButton.setOnPress([this]() {
        power = max(0.0f, power - 10.0f);
        updatePowerLabel();
        if (on) applyOutput();
    });

    powerPlusButton.setOnPress([this]() {
        power = min(100.0f, power + 10.0f);
        updatePowerLabel();
        if (on) applyOutput();
    });

    directionButton.setOnPress([this]() {
        clockwise = !clockwise;
        directionButton.setText(clockwise ? "CW" : "CCW");
        if (on) applyOutput();
    });

    toggleButton.setOnPress([this]() {
        on = !on;
        toggleButton.setText(on ? "OFF" : "ON");
        applyOutput();
    });

    widgets = {
        &panel,
        &modeLabel, &spindleModeButton, &laserModeButton,
        &powerLabel, &powerValueLabel, &powerMinusButton, &powerPlusButton,
        &directionButton, &toggleButton
    };
}

void ToolsScreen::selectMode(ToolMode newMode)
{
    if (mode == newMode)
        return;
    if (on)
    {
        on = false;
        toggleButton.setText("ON");
        grbl.setSpindlePower(0.0f);
    }

    mode = newMode;

    spindleModeButton.setSelected(mode == ToolMode::Spindle);
    laserModeButton.setSelected(mode == ToolMode::Laser);

    updatePowerLabel();
}

void ToolsScreen::updatePowerLabel()
{
    if (mode == ToolMode::Spindle)
    {
        int rpm = (int)(power / 100.0f * MAX_SPINDLE_SPEED);
        powerValueLabel.setText(String(rpm) + " RPM");
    }
    else
    {
        powerValueLabel.setText(String((int)power) + "%");
    }
}

void ToolsScreen::applyOutput()
{
    float outputPower = on ? power : 0.0f;
    grbl.setSpindlePower(outputPower, mode == ToolMode::Spindle ? clockwise : true);
}