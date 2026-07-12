#pragma once

#include <functional>
#include "../../core/widget.h"
#include "../../theme.h"

class ToggleWidget : public Widget
{
public:
    using ChangeCallback = std::function<void(bool)>;

    ToggleWidget(const Rect& bounds, bool initialState = false);

    void draw(DisplayManager& display) override;
    bool handleTouch(const TouchEvent& event) override;

    void setState(bool value);
    bool getState() const;

    void setOnChange(ChangeCallback callback);

private:
    bool state;
    ChangeCallback onChange;
};