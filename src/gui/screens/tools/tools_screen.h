#pragma once
#include <functional>
#include "../../core/screen.h"
#include "../../core/scroll_panel_widget.h"
#include "../../theme.h"
#include "../../widgets/label/label_widget.h"
#include "../../widgets/button/button_widget.h"
#include "../../widgets/icon_button/icon_button_widget.h"
#include "../../widgets/menu_row/menu_row_widget.h"
#include "../../widgets/keyboard/keyboard_modal_widget.h"
#include "../../widgets/numeric_pad/numeric_pad_modal_widget.h"
#include "../../../machine/grbl_controller.h"
#include "../../../machine/tool_profile_manager.h"
#include "../../widgets/toggle/toggle_widget.h"
enum class ToolsPage
{
    List,
    Detail
};
enum class ToolsNumericTarget
{
    Diameter,
    Power
};

class ToolsScreen : public IScreen
{
public:
    ToolsScreen(GrblController &grblController);

    void update() override;
    void draw(DisplayManager &display) override;

    bool isKeyboardVisible() const { return nameKeyboard.isVisible(); }
    void drawKeyboard(DisplayManager &display) { nameKeyboard.draw(display); }
    bool handleKeyboardTouch(const TouchEvent &event) { return nameKeyboard.handleTouch(event); }

    bool isNumericPadVisible() const { return numericPad.isVisible(); }
    void drawNumericPad(DisplayManager &display) { numericPad.draw(display); }
    bool handleNumericPadTouch(const TouchEvent &event) { return numericPad.handleTouch(event); }
    ToolProfileManager &getToolManager() { return toolManager; }

    using DeleteConfirmCallback = std::function<void(std::function<void()>)>;
    void setOnRequestDeleteConfirm(DeleteConfirmCallback callback);

    void syncLaserModeWithActiveTool();
    void requestModalsRedraw()
    {
        nameKeyboard.requestRedraw();
        numericPad.requestRedraw();
    }
    void onExit() override;

private:
    GrblController &grbl;
    ToolProfileManager toolManager;

    ToolsPage currentPage = ToolsPage::List;
    bool needsClear = false;
    int8_t selectedIndex = -1;

    // List - scroll
    ScrollPanelWidget listScrollPanel;
    static constexpr uint8_t MAX_ROWS = 8;
    MenuRowWidget profileRows[MAX_ROWS];
    uint8_t rowCount = 0;
    ButtonWidget addButton;
    // detail scroll
    ScrollPanelWidget detailScrollPanel;
    IconButtonWidget backButton;
    LabelWidget nameLabel;
    MenuRowWidget diameterRow;
    MenuRowWidget powerRow;
    MenuRowWidget nameRow;
    LabelWidget usageLabel;
    LabelWidget usageValueLabel;

    ButtonWidget spindleTypeButton;
    ButtonWidget laserTypeButton;

    ButtonWidget directionButton;
    ToggleWidget testToggle;
    ToggleWidget activeToggles[MAX_ROWS];
    LabelWidget testLabelToggle;
    DeleteConfirmCallback onRequestDeleteConfirm;
    ButtonWidget deleteButton;

    KeyboardModalWidget nameKeyboard;
    NumericPadModalWidget numericPad;
    ToolsNumericTarget numericTarget = ToolsNumericTarget::Diameter;

    ToolProfile editingProfile;
    bool testOn = false;
    bool testClockwise = true;
    unsigned long testStartedAt = 0;

    void rebuildList();
    void openDetail(int8_t index);
    void refreshDetailLabels();
    void applyTestOutput();
    void stopTestIfRunning();
    void applyLaserModeForTool(const ToolProfile &tool);
};