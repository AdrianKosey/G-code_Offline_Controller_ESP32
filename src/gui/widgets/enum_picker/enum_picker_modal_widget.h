#pragma once

#include <Arduino.h>
#include <functional>

#include "../../core/rect.h"
#include "../../widgets/panel/panel_widget.h"
#include "../../widgets/label/label_widget.h"
#include "../../widgets/button/button_widget.h"
#include "../../theme.h"

class EnumPickerModalWidget
{
public:
    using SubmitCallback = std::function<void(uint8_t)>;

    static constexpr uint8_t MAX_OPTIONS = 8;

    EnumPickerModalWidget(const Rect& screenBounds);

    void show(const String& title, const char** labels, uint8_t count, uint8_t currentIndex);
    void hide();
    bool isVisible() const;

    void draw(DisplayManager& display);
    bool handleTouch(const TouchEvent& event);

    void setOnSubmit(SubmitCallback callback);

private:
    Rect screenBounds;
    bool visible = false;
    bool dirty = false;

    PanelWidget panel;
    LabelWidget titleLabel;
    ButtonWidget optionButtons[MAX_OPTIONS];
    uint8_t optionCount = 0;

    SubmitCallback onSubmit;
};