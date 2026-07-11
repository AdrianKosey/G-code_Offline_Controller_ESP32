#pragma once

#include "../../core/rect.h"
#include "../../widgets/panel/panel_widget.h"
#include "../../widgets/label/label_widget.h"
#include "../../theme.h"

class LoadingModalWidget
{
public:
    LoadingModalWidget(const Rect& screenBounds);

    void show(const String& message);
    void hide();
    bool isVisible() const;

    void draw(DisplayManager& display);
    bool handleTouch(const TouchEvent& event);

private:
    Rect screenBounds;
    bool visible = false;
    bool dirty = false;
    PanelWidget panel;
    LabelWidget messageLabel;
};