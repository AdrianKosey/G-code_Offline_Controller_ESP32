#pragma once

#include <Arduino.h>
#include <functional>

#include "../../core/rect.h"
#include "../../widgets/panel/panel_widget.h"
#include "../../widgets/label/label_widget.h"
#include "../../widgets/button/button_widget.h"
#include "../../theme.h"

class NumericPadModalWidget
{
public:
    using SubmitCallback = std::function<void(float)>;
    using CancelCallback = std::function<void()>;

    NumericPadModalWidget(const Rect& screenBounds);

    void show(const String& title, float currentValue);
    void hide();
    bool isVisible() const;

    void draw(DisplayManager& display);
    bool handleTouch(const TouchEvent& event);

    void setOnSubmit(SubmitCallback callback);
    void setOnCancel(CancelCallback callback);

private:
    Rect screenBounds;
    bool visible = false;
    bool dirty = false;

    PanelWidget panel;
    LabelWidget titleLabel;
    LabelWidget inputLabel;

    ButtonWidget digitButtons[10];
    ButtonWidget dotButton;
    ButtonWidget signButton;
    ButtonWidget backspaceButton;
    ButtonWidget cancelButton;
    ButtonWidget submitButton;

    String inputText;

    SubmitCallback onSubmit;
    CancelCallback onCancel;

    void appendChar(char c);
};