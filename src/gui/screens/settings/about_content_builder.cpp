#include "about_content_builder.h"
#include "../../../../include/config.h"
#include "../../../i18n/translations.h"
namespace
{
    float rssiToPercent(int32_t rssi)
    {
        constexpr int32_t RSSI_MIN = -90;
        constexpr int32_t RSSI_MAX = -30;

        int32_t clamped = constrain(rssi, RSSI_MIN, RSSI_MAX);
        return (float)(clamped - RSSI_MIN) / (float)(RSSI_MAX - RSSI_MIN) * 100.0f;
    }
}

AboutContentBuilder::AboutContentBuilder(GrblController& grblController, WifiManager& wifiManager)
    : grbl(grblController), wifi(wifiManager),

      controlNameValue(Rect{0, 0, 220, 18}, "", Theme::Text, 2, Theme::Background, true),
      cncVersionValue(Rect{0, 0, 220, 18}, "", Theme::Text, 2, Theme::Background, true),

      sdLabel(Rect{0, 0, 150, 16}, tr(StringId::Settings_SD), Theme::TextSecondary, 1, Theme::Background, false),
      sdBar(Rect{0, 0, 220, 10}, Theme::SidebarBackground, Theme::Progress),
      sdValue(Rect{0, 0, 220, 16}, "", Theme::TextSecondary, 1, Theme::Background, false),
      controlNameCaption(Rect{0,0,150,14}, tr(StringId::Settings_NameDevice), Theme::TextSecondary, 1, Theme::Background, false),
      cncVersionCaption(Rect{0,0,150,14}, tr(StringId::Settings_VersionCNC), Theme::TextSecondary, 1, Theme::Background, false),
      tempCaption(Rect{0,0,150,14}, tr(StringId::Settings_Temperature), Theme::TextSecondary, 1, Theme::Background, false),
      uptimeCaption(Rect{0,0,150,14}, tr(StringId::Settings_TimeOn), Theme::TextSecondary, 1, Theme::Background, false),
      ramCaption(Rect{0,0,150,14}, tr(StringId::Settings_RAM), Theme::TextSecondary, 1, Theme::Background, false),
      ramBar(Rect{0, 0, 220, 8}, Theme::SidebarBackground, Theme::Progress2),
      cpuFreqCaption(Rect{0,0,150,14}, tr(StringId::Settings_FrequencyCPU), Theme::TextSecondary, 1, Theme::Background, false),
      baudCaption(Rect{0,0,150,14}, tr(StringId::Settings_Serial), Theme::TextSecondary, 1, Theme::Background, false),
      wifiSignalCaption(Rect{0,0,150,14}, tr(StringId::Settings_WiFiSignal), Theme::TextSecondary, 1, Theme::Background, false),
      wifiSignalBar(Rect{0, 0, 220, 8}, Theme::SidebarBackground, Theme::Progress3),
      ipCaption(Rect{0,0,150,14}, tr(StringId::Settings_IP), Theme::TextSecondary, 1, Theme::Background, false),
      macCaption(Rect{0,0,150,14}, tr(StringId::Settings_MAC), Theme::TextSecondary, 1, Theme::Background, false),

      tempValue(Rect{0, 0, 220, 18}, "", Theme::Text, 2, Theme::Background, false),
      uptimeValue(Rect{0, 0, 220, 18}, "", Theme::Text, 2, Theme::Background, false),
      ramValue(Rect{0, 0, 220, 16}, "", Theme::TextSecondary, 1, Theme::Background, true),
      cpuFreqValue(Rect{0, 0, 220, 18}, "", Theme::Text, 2, Theme::Background, false),
      baudValue(Rect{0, 0, 220, 18}, "", Theme::Text, 2, Theme::Background, false),
      wifiSignalValue(Rect{0, 0, 220, 16}, "", Theme::TextSecondary, 1, Theme::Background, false),
      ipValue(Rect{0, 0, 220, 18}, "", Theme::Text, 2, Theme::Background, true),
      macValue(Rect{0, 0, 220, 18}, "", Theme::Text, 2, Theme::Background, true)
{}

String AboutContentBuilder::formatBytes(uint64_t bytes)
{
    if (bytes >= 1073741824ULL)
        return String((double)bytes / 1073741824.0, 2) + " GB";

    return String((double)bytes / 1048576.0, 1) + " MB";
}

String AboutContentBuilder::formatUptime(unsigned long seconds)
{
    unsigned long h = seconds / 3600;
    unsigned long m = (seconds % 3600) / 60;
    unsigned long s = seconds % 60;

    char buf[16];
    snprintf(buf, sizeof(buf), "%02lu:%02lu:%02lu", h, m, s);
    return String(buf);
}

void AboutContentBuilder::refresh()
{
    controlNameValue.setText(String(CONTROL_NAME) + " " + CONTROL_VERSION);
    cncVersionValue.setText(grbl.getFirmwareVersion());

    DeviceInfo info = DeviceInfoProvider::collect(true);

    if (info.sdAvailable && info.sdTotalBytes > 0)
    {
        float percent = (float)info.sdUsedBytes / (float)info.sdTotalBytes * 100.0f;
        sdBar.setValue(percent);
        sdValue.setText(formatBytes(info.sdUsedBytes) + " / " + formatBytes(info.sdTotalBytes));
    }
    else
    {
        sdBar.setValue(0);
        sdValue.setText(tr(StringId::Files_NoSdCard));
    }

    tempValue.setText(String(info.temperatureC, 1) + " C");
    uptimeValue.setText(formatUptime(info.uptimeSeconds));

    uint32_t heapUsed = info.heapTotal - info.heapFree;
    float ramPercent = (float)heapUsed / (float)info.heapTotal * 100.0f;
    ramBar.setValue(ramPercent); // <- nuevo
    ramValue.setText(formatBytes(heapUsed) + " / " + formatBytes(info.heapTotal));

    cpuFreqValue.setText(String(info.cpuFreqMHz) + " MHz");
    baudValue.setText("115200 baud");

    if (info.wifiConnected)
    {
        wifiSignalBar.setValue(rssiToPercent(info.wifiRssi)); // <- nuevo
        wifiSignalValue.setText(String(info.wifiRssi) + " dBm");
    }
    else
    {
        wifiSignalBar.setValue(0); // <- nuevo
        wifiSignalValue.setText(tr(StringId::Settings_NotConnected));
    }

    ipValue.setText(info.ip);
    macValue.setText(info.mac);
}

void AboutContentBuilder::attachTo(ScrollPanelWidget& panel)
{
    int16_t y = 0;
    constexpr int16_t rowH = 40;

    auto addRow = [&](Widget& caption, Widget& value)
    {
        panel.addChild(&caption, 8, y);
        panel.addChild(&value, 8, y + 14);
        y += rowH;
    };

    addRow(controlNameCaption, controlNameValue);
    addRow(cncVersionCaption, cncVersionValue);

    panel.addChild(&sdLabel, 8, y);
    panel.addChild(&sdBar, 8, y + 16);
    panel.addChild(&sdValue, 8, y + 28);
    y += rowH + 10;

    addRow(tempCaption, tempValue);
    addRow(uptimeCaption, uptimeValue);

    // RAM - con barra, igual formato que SD
    panel.addChild(&ramCaption, 8, y);
    panel.addChild(&ramBar, 8, y + 16);
    panel.addChild(&ramValue, 8, y + 28);
    y += rowH + 10;

    addRow(cpuFreqCaption, cpuFreqValue);
    addRow(baudCaption, baudValue);

    // Wi-Fi signal - con barra
    panel.addChild(&wifiSignalCaption, 8, y);
    panel.addChild(&wifiSignalBar, 8, y + 16);
    panel.addChild(&wifiSignalValue, 8, y + 28);
    y += rowH + 10;

    addRow(ipCaption, ipValue);
    addRow(macCaption, macValue);
}