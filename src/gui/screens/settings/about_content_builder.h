#pragma once

#include "../../core/scroll_panel_widget.h"
#include "../../widgets/label/label_widget.h"
#include "../../widgets/progress_bar/progress_bar_widget.h"
#include "../../../machine/grbl_controller.h"
#include "../../../app/device_info_provider.h"
#include "../../../network/wifi_manager.h"

class AboutContentBuilder
{
public:
    AboutContentBuilder(GrblController& grbl, WifiManager& wifi);

    void attachTo(ScrollPanelWidget& panel); 
    void refresh(); 

private:
    GrblController& grbl;
    WifiManager& wifi;

    LabelWidget controlNameValue;
    LabelWidget cncVersionValue;
    LabelWidget controlNameCaption, cncVersionCaption;
    LabelWidget tempCaption, uptimeCaption, ramCaption, cpuFreqCaption, baudCaption, wifiSignalCaption, ipCaption, macCaption;
    LabelWidget sdLabel;
    ProgressBarWidget sdBar;
    LabelWidget sdValue;

    LabelWidget tempValue;
    LabelWidget uptimeValue;
    LabelWidget ramValue;
    ProgressBarWidget ramBar;
    LabelWidget cpuFreqValue;
    LabelWidget baudValue;
    LabelWidget wifiSignalValue;
    ProgressBarWidget wifiSignalBar;
    LabelWidget ipValue;
    LabelWidget macValue;

    static String formatBytes(uint64_t bytes);
    static String formatUptime(unsigned long seconds);
};