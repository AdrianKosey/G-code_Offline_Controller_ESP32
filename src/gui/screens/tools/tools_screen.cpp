#include "tools_screen.h"
#include "../../../i18n/translations.h"
#include "../../icons.h"
static constexpr int16_t CONTENT_X = 60;
static constexpr int16_t CONTENT_Y = 34;
static constexpr int16_t CONTENT_WIDTH = 260;
static constexpr int16_t CONTENT_HEIGHT = 206;

ToolsScreen::ToolsScreen(GrblController& grblController)
    : grbl(grblController),

      listScrollPanel(Rect{CONTENT_X + 8, CONTENT_Y + 8, 244, 198}),
      addButton(Rect{0, 0, 244, 32}, tr(StringId::Tool_New)),
      detailScrollPanel(Rect{CONTENT_X + 8, CONTENT_Y + 36, 244, 170}),
      backButton(
          Rect{CONTENT_X + 8, CONTENT_Y + 4, 28, 24},
          Icons::Back, Icons::HEADER_WIDTH, Icons::HEADER_HEIGHT,
          Theme::Background, Theme::Text, false),
      nameLabel(Rect{CONTENT_X + 44, CONTENT_Y + 4, 180, 20}, "", Theme::Text, 2, Theme::Background, true),

      diameterRow(Rect{0, 0, 244, 36}, tr(StringId::Tool_Diameter)),
      nameRow(Rect{0, 0, 244, 36}, "Name"),
      powerRow(Rect{0, 0, 244, 36}, tr(StringId::Tools_Power)),

      usageLabel(Rect{0, 0, 140, 14}, tr(StringId::Tool_Usage), Theme::TextSecondary, 1, Theme::Background, false),
      usageValueLabel(Rect{0, 0, 200, 18}, "00:00:00", Theme::Text, 2, Theme::Background, false),

      spindleTypeButton(Rect{0, 0, 118, 28}, tr(StringId::Tools_Spindle)),
      laserTypeButton(Rect{0, 0, 118, 28}, tr(StringId::Tools_Laser)),

      directionButton(Rect{0, 0, 118, 32}, tr(StringId::Tools_CW)),
      testToggle(Rect{0, 0, 48, 24}, false),
      testLabelToggle(Rect{0, 0, 100, 24}, tr(StringId::Tools_OnOff), Theme::Text, 1, Theme::Background, false),
      deleteButton(Rect{0, 0, 244, 28}, tr(StringId::Tool_Delete)),

      nameKeyboard(Rect{0, 0, 320, 240}),
      numericPad(Rect{0, 0, 320, 240})
{
    addButton.setOnPress([this]() {
        editingProfile = ToolProfile{};
        editingProfile.name = tr(StringId::Tool_Count) + String(toolManager.getProfiles().size() + 1);
        toolManager.addProfile(editingProfile);
        openDetail((int8_t)(toolManager.getProfiles().size() - 1));
    });
    backButton.setOnPress([this]() {
        stopTestIfRunning();
        rebuildList();
    });

    diameterRow.setOnPress([this]() {
        numericTarget = ToolsNumericTarget::Diameter;
        numericPad.show(tr(StringId::Tool_Diameter), editingProfile.diameterMm);
    });

    nameRow.setOnPress([this]() {
        nameKeyboard.show("Tool Name", editingProfile.name);
    });
    nameKeyboard.setOnSubmit([this](const String& newName) {
        if (newName.length() == 0)
            return;

        editingProfile.name = newName;
        toolManager.updateProfile(selectedIndex, editingProfile);
        refreshDetailLabels();
    });
    nameKeyboard.setOnCancel([this]() {});

    powerRow.setOnPress([this]() {
        numericTarget = ToolsNumericTarget::Power;
        numericPad.show(tr(StringId::Tools_Power), editingProfile.defaultPower);
    });

    numericPad.setOnSubmit([this](float value) {
        if (numericTarget == ToolsNumericTarget::Diameter)
            editingProfile.diameterMm = max(0.1f, value);
        else
            editingProfile.defaultPower = constrain(value, 0.0f, 100.0f);

        toolManager.updateProfile(selectedIndex, editingProfile);
        refreshDetailLabels();
    });

    numericPad.setOnCancel([this]() {});

    spindleTypeButton.setOnPress([this]() {
        stopTestIfRunning();
        editingProfile.type = ToolType::Spindle;
        toolManager.updateProfile(selectedIndex, editingProfile);
        if (selectedIndex == toolManager.getActiveIndex())
            applyLaserModeForTool(editingProfile);
        spindleTypeButton.setSelected(true);
        laserTypeButton.setSelected(false);
        detailScrollPanel.invalidate();
        refreshDetailLabels();
    });

    laserTypeButton.setOnPress([this]() {
        stopTestIfRunning();
        editingProfile.type = ToolType::Laser;
        toolManager.updateProfile(selectedIndex, editingProfile);
        if (selectedIndex == toolManager.getActiveIndex())
            applyLaserModeForTool(editingProfile);
        spindleTypeButton.setSelected(false);
        laserTypeButton.setSelected(true);
        detailScrollPanel.invalidate();
        refreshDetailLabels();
    });

    directionButton.setOnPress([this]() {
        testClockwise = !testClockwise;
        directionButton.setText(testClockwise ? tr(StringId::Tools_CW) : tr(StringId::Tools_CCW));
        if (testOn) applyTestOutput();
    });

    testToggle.setOnChange([this](bool state) {
        testOn = state;

        if (testOn)
            testStartedAt = millis();
        else
            stopTestIfRunning();

        applyTestOutput();
    });

    deleteButton.setOnPress([this]() {
        if (onRequestDeleteConfirm)
        {
            onRequestDeleteConfirm([this]() {
                stopTestIfRunning();
                toolManager.deleteProfile(selectedIndex);
                rebuildList();
            });
        }
        else{}
    });
    
    detailScrollPanel.addChild(&nameRow, 0, 0);
    detailScrollPanel.addChild(&diameterRow, 0, 38);
    detailScrollPanel.addChild(&powerRow, 0, 76);
    detailScrollPanel.addChild(&usageLabel, 8, 122);
    detailScrollPanel.addChild(&usageValueLabel, 8, 136);
    detailScrollPanel.addChild(&spindleTypeButton, 0, 168);
    detailScrollPanel.addChild(&laserTypeButton, 126, 168);
    detailScrollPanel.addChild(&directionButton, 0, 202);
    detailScrollPanel.addChild(&testToggle, 126, 202);
    detailScrollPanel.addChild(&testLabelToggle, 180, 202);
    detailScrollPanel.addChild(&deleteButton, 0, 244);

    rebuildList();
}

void ToolsScreen::rebuildList()
{
    const auto& profiles = toolManager.getProfiles();
    rowCount = min((uint8_t)profiles.size(), MAX_ROWS);

    listScrollPanel = ScrollPanelWidget(Rect{CONTENT_X + 8, CONTENT_Y + 8, 244, 198});

    int8_t activeIndex = toolManager.getActiveIndex();

    for (uint8_t i = 0; i < rowCount; i++)
    {
        char buf[16];
        snprintf(buf, sizeof(buf), "%.1fmm", profiles[i].diameterMm);

        profileRows[i] = MenuRowWidget(Rect{0, 0, 190, 36}, profiles[i].name);
        profileRows[i].setValue(String(buf));

        uint8_t idx = i;
        profileRows[i].setOnPress([this, idx]() { openDetail(idx); });

        activeToggles[i] = ToggleWidget(Rect{0, 0, 44, 22}, i == activeIndex);

        activeToggles[i].setOnChange([this, idx](bool state) {
            if (state)
            {
                toolManager.setActiveIndex(idx);
                applyLaserModeForTool(toolManager.getProfiles()[idx]);

                for (uint8_t j = 0; j < rowCount; j++)
                {
                    if (j != idx)
                        activeToggles[j].setState(false);
                }
            }
            else if (toolManager.getActiveIndex() == idx)
            {
                toolManager.setActiveIndex(-1);
            }
        });

        listScrollPanel.addChild(&profileRows[i], 0, i * 40);
        listScrollPanel.addChild(&activeToggles[i], 200, i * 40 + 7);
    }

    listScrollPanel.addChild(&addButton, 0, rowCount * 40 + 6);

    currentPage = ToolsPage::List;
    needsClear = true;

    widgets = { &listScrollPanel };
    invalidateAll();
}

void ToolsScreen::openDetail(int8_t index)
{
    const auto& profiles = toolManager.getProfiles();
    if (index < 0 || index >= (int8_t)profiles.size())
        return;

    selectedIndex = index;
    editingProfile = profiles[index];
    testOn = false;
    testClockwise = true;

    spindleTypeButton.setSelected(editingProfile.type == ToolType::Spindle);
    laserTypeButton.setSelected(editingProfile.type == ToolType::Laser);
    directionButton.setText(tr(StringId::Tools_CW));

    refreshDetailLabels();

    currentPage = ToolsPage::Detail;
    needsClear = true;

    widgets = { &backButton, &nameLabel, &detailScrollPanel };
    invalidateAll();
}

void ToolsScreen::refreshDetailLabels()
{
    nameLabel.setText(editingProfile.name);
    nameRow.setValue(editingProfile.name);
    diameterRow.setValue(String(editingProfile.diameterMm, 1) + " mm");
    powerRow.setValue(String((int)editingProfile.defaultPower) + "%");

    uint32_t sec = editingProfile.usageSeconds;
    char buf[16];
    snprintf(buf, sizeof(buf), "%02u:%02u:%02u", sec / 3600, (sec % 3600) / 60, sec % 60);
    usageValueLabel.setText(String(buf));

    //directionButton.setEnabled(editingProfile.type == ToolType::Spindle); // NO FUNTION 
}

void ToolsScreen::applyTestOutput()
{
    float outputPower = testOn ? editingProfile.defaultPower : 0.0f;
    grbl.setSpindlePower(outputPower, editingProfile.type == ToolType::Spindle ? testClockwise : true);

    if (testOn && editingProfile.type == ToolType::Laser && grbl.isLaserMode())
    {
        // Grbl in laser mode does not apply PWM until the planner processes
        // an actual movement—we force a zero-distance move to
        // "release" the queued M3 command without actually moving the machine.
        grbl.sendLine("G91 G1 X0 Y0 F1");
        grbl.sendLine("G90");
    }
}

void ToolsScreen::stopTestIfRunning()
{
    if (!testOn)
        return;

    testOn = false;
    testToggle.setState(false);
    grbl.setSpindlePower(0.0f);

    uint32_t elapsed = (millis() - testStartedAt) / 1000;
    toolManager.addUsageSeconds(selectedIndex, elapsed);

    refreshDetailLabels();
}

void ToolsScreen::onExit()
{
    stopTestIfRunning();
}

void ToolsScreen::update()
{
    IScreen::update();

    // While the test is running, visually refresh the hour meter every second,
    // without writing to flash memory until it stops (to avoid unnecessary wear)
    if (testOn)
    {
        static unsigned long lastRefresh = 0;
        if (millis() - lastRefresh > 1000)
        {
            lastRefresh = millis();

            uint32_t liveSeconds = editingProfile.usageSeconds + (millis() - testStartedAt) / 1000;
            char buf[16];
            snprintf(buf, sizeof(buf), "%02u:%02u:%02u", liveSeconds / 3600, (liveSeconds % 3600) / 60, liveSeconds % 60);
            usageValueLabel.setText(String(buf));
        }
    }
}

void ToolsScreen::draw(DisplayManager& display)
{
    if (needsClear)
    {
        display.fillRect(CONTENT_X, CONTENT_Y, CONTENT_WIDTH, CONTENT_HEIGHT, Theme::Background);
        needsClear = false;
    }

    IScreen::draw(display);
}

void ToolsScreen::setOnRequestDeleteConfirm(DeleteConfirmCallback callback)
{
    onRequestDeleteConfirm = callback;
}

void ToolsScreen::applyLaserModeForTool(const ToolProfile& tool)
{
    // Grbl rejects $ changes while the machine is not in Idle (error:8),
    // and changing the mode mid-operation would be dangerous anyway.
    GrblState state = grbl.getStatus().state;
    if (state != GrblState::Idle && state != GrblState::Unknown)
        return;

    bool wantLaser = (tool.type == ToolType::Laser);

    // Only write if it actually changes - every write of $ goes to Grbl's EEPROM
    if (grbl.isLaserMode() == wantLaser)
        return;

    grbl.setSetting(32, wantLaser ? 1.0f : 0.0f);
}

void ToolsScreen::syncLaserModeWithActiveTool()
{
    if (toolManager.getActiveIndex() < 0)
        return;

    applyLaserModeForTool(toolManager.getActiveTool());
}