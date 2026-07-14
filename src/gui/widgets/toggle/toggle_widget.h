#pragma once

#include <functional>
#include "../../core/widget.h"
#include "../../theme.h"


void drawToggleVisual(DisplayManager& display, const Rect& bounds, bool state);

class ToggleWidget : public Widget
{
public:
    using ChangeCallback = std::function<void(bool)>;

    ToggleWidget(const Rect& bounds, bool initialState = false);

    void draw(DisplayManager& display) override;
    bool handleTouch(const TouchEvent& event) override;

    void setState(bool value);
    bool getState() const;
    void setEnabled(bool enabled);
    bool isEnabled() const;

    void setOnChange(ChangeCallback callback);

private:
    bool state;
    bool enabled = true;
    ChangeCallback onChange;
};