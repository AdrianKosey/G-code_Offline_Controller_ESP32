#pragma once

#include <Arduino.h>
#include <functional>

#include "../../core/rect.h"
#include "../../widgets/panel/panel_widget.h"
#include "../../widgets/label/label_widget.h"
#include "../../widgets/menu_row/menu_row_widget.h"
#include "../../widgets/icon_button/icon_button_widget.h"
#include "../../widgets/console_log/console_log_widget.h"
#include "../../widgets/keyboard/keyboard_modal_widget.h"
#include "../../../machine/grbl_controller.h"
#include "../../theme.h"

class GcodeConsoleModalWidget
{
public:
    GcodeConsoleModalWidget(const Rect& screenBounds, GrblController& grbl);

    void show(DisplayManager& display);
    void hide();
    bool isVisible() const;

    void update(); // sincroniza el log con GrblController
    void draw(DisplayManager& display);
    bool handleTouch(const TouchEvent& event);

    void requestRedraw()
    {
        if (visible) dirty = true;
        commandKeyboard.requestRedraw();
    }

private:
    Rect screenBounds;
    GrblController& grbl;

    bool visible = false;
    bool dirty = false;

    PanelWidget panel;
    LabelWidget titleLabel;
    IconButtonWidget closeButton;
    ConsoleLogWidget consoleLog;
    MenuRowWidget commandInputRow;

    KeyboardModalWidget commandKeyboard;
};