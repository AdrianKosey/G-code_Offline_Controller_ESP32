#pragma once

#include "../widgets/label/label_widget.h"
#include "../widgets/icon/icon_widget.h"
#include "../widgets/icon_button/icon_button_widget.h"
#include "../icons.h"
#include "../theme.h"
#include <functional>
class HeaderWidget
{
public:
    HeaderWidget(const Rect &bounds);

    void draw(DisplayManager &display);

    void setTitle(const String &title);
    void setSdStatus(bool ready);
    void setWifiStatus(bool connected);
    void setMachineStatus(bool connected);
    void setUsbStatus(bool connected);
    void invalidate()
    {
        titleLabel.invalidate();
        sdIcon.invalidate();
        usbIcon.invalidate();
        wifiIcon.invalidate();
        machineIcon.invalidate();
        consoleButton.invalidate();
    }

    void setOnConsolePress(std::function<void()> callback);
    bool handleTouch(const TouchEvent& event);
private:
    LabelWidget titleLabel;
    IconWidget sdIcon;
    IconWidget wifiIcon;
    IconWidget machineIcon;
    IconWidget usbIcon;
    IconButtonWidget consoleButton;
};