#pragma once

#include <Arduino.h>
#include <functional>

#include "../../core/rect.h"
#include "../panel/panel_widget.h"
#include "../text_area/text_area_widget.h"
#include "../button/button_widget.h"
#include "../../theme.h"

class InfoModalWidget
{
public:
    using CloseCallback = std::function<void()>;

    InfoModalWidget(const Rect& screenBounds);

    void show(DisplayManager& display, const String& message);
    void hide();
    bool isVisible() const;

    void draw(DisplayManager& display);
    bool handleTouch(const TouchEvent& event);

    void setOnClose(CloseCallback callback);
    void requestRedraw() { if (visible) dirty = true; }

private:
    Rect screenBounds;
    bool visible = false;
    bool dirty = false;

    PanelWidget panel;
    TextAreaWidget messageArea;
    ButtonWidget okButton;

    CloseCallback onClose;
};