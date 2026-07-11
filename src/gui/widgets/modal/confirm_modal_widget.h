#pragma once

#include <Arduino.h>
#include <functional>

#include "../../core/rect.h"
#include "../../widgets/panel/panel_widget.h"
#include "../../widgets/label/label_widget.h"
#include "../../widgets/button/button_widget.h"
#include "../../theme.h"

class ConfirmModalWidget
{
public:
    using ConfirmCallback = std::function<void()>;

    ConfirmModalWidget(const Rect& screenBounds);

    void show(const String& message);
    void hide();
    bool isVisible() const;

    void draw(DisplayManager& display);
    bool handleTouch(const TouchEvent& event); 

    void setOnConfirm(ConfirmCallback callback);
    void setOnCancel(ConfirmCallback callback);

private:
    Rect screenBounds;
    bool dirty = false;
    bool visible = false;

    PanelWidget panel;
    LabelWidget messageLabel;
    ButtonWidget yesButton;
    ButtonWidget noButton;

    ConfirmCallback onConfirm;
    ConfirmCallback onCancel;
};