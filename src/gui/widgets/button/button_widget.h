#pragma once

#include <Arduino.h>
#include "../../core/widget.h"
#include "../../theme.h"

class ButtonWidget : public Widget
{
public:
    ButtonWidget(const Rect &bounds, const String &text);

    void draw(DisplayManager &display) override;

    bool handleTouch(const TouchEvent &event) override;

    bool contains(uint16_t x, uint16_t y) const;

    void setSelected(bool selected);

    bool isSelected() const;

    void setText(const String &text);

private:
    String text;

    bool selected = false;
};