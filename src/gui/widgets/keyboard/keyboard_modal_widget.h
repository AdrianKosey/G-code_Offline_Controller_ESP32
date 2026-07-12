#pragma once

#include <Arduino.h>
#include <functional>

#include "../../core/rect.h"
#include "../../widgets/panel/panel_widget.h"
#include "../../widgets/label/label_widget.h"
#include "../../widgets/button/button_widget.h"
#include "../../theme.h"

enum class KeyboardMode
{
    Letters,
    Symbols
};

class KeyboardModalWidget
{
public:
    using SubmitCallback = std::function<void(const String&)>;
    using CancelCallback = std::function<void()>;

    static constexpr uint8_t MAX_KEYS = 30;

    KeyboardModalWidget(const Rect& screenBounds);

    void show(const String& title);
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

    ButtonWidget shiftButton;
    ButtonWidget modeToggleButton;
    ButtonWidget spaceButton;
    ButtonWidget backspaceButton;
    ButtonWidget cancelButton;
    ButtonWidget submitButton;

    ButtonWidget keys[MAX_KEYS];
    char keyChars[MAX_KEYS];
    uint8_t keyCount = 0;

    String inputText;
    bool shiftActive = false;
    KeyboardMode mode = KeyboardMode::Letters;

    SubmitCallback onSubmit;
    CancelCallback onCancel;

    void buildLayout();
    void refreshKeyLabels();
    void appendChar(char c);
    void toggleMode();
};