#pragma once

#include "../widgets/label/label_widget.h"
#include "../widgets/icon/icon_widget.h"
#include "../icons.h"
#include "../theme.h"

class HeaderWidget
{
public:
    HeaderWidget(const Rect& bounds);

    void draw(DisplayManager& display);

    void setTitle(const String& title);
    void setSdStatus(bool ready);
    void setWifiStatus(bool connected);

private:
    LabelWidget titleLabel;
    IconWidget sdIcon;
    IconWidget wifiIcon;
};