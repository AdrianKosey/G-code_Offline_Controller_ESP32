#pragma once

#include <functional>
#include "../../core/widget.h"
#include "../../theme.h"

class MenuRowWidget : public Widget
{
public:
    using PressCallback = std::function<void()>;
    MenuRowWidget() = default;
    MenuRowWidget(const Rect& bounds, const String& label);

    void draw(DisplayManager& display) override;
    bool handleTouch(const TouchEvent& event) override;

    void setOnPress(PressCallback callback);

private:
    String label;
    PressCallback onPress;
    bool pressed = false;
};