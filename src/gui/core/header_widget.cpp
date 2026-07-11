#include "header_widget.h"

HeaderWidget::HeaderWidget(const Rect& bounds)
    : titleLabel(Rect{static_cast<short>(bounds.x + 10), static_cast<short>(bounds.y + 4), static_cast<short>(150), static_cast<short>(20)}, "", Theme::Text, static_cast<short>(2), Theme::Background),
      sdIcon(Rect{static_cast<short>(bounds.x + bounds.width - 46), static_cast<short>(bounds.y + 6), Icons::HEADER_WIDTH, Icons::HEADER_HEIGHT}, Icons::MicroSd, Icons::HEADER_WIDTH, Icons::HEADER_HEIGHT, Theme::TextSecondary, Theme::Background),
      wifiIcon(Rect{static_cast<short>(bounds.x + bounds.width - 22), static_cast<short>(bounds.y + 6), Icons::HEADER_WIDTH, Icons::HEADER_HEIGHT}, Icons::Wifi, Icons::HEADER_WIDTH, Icons::HEADER_HEIGHT, Theme::TextSecondary, Theme::Background)
{}

void HeaderWidget::setTitle(const String& title)
{
    titleLabel.setText(title);
}

void HeaderWidget::setSdStatus(bool ready)
{
    sdIcon.setIcon(ready ? Icons::MicroSd : Icons::NoMicroSd);
    sdIcon.setColor(ready ? Theme::TextSecondary : Theme::Error);
}

void HeaderWidget::setWifiStatus(bool connected)
{
    wifiIcon.setIcon(connected ? Icons::Wifi : Icons::NoWifi);
    wifiIcon.setColor(connected ? Theme::TextSecondary : Theme::Error);
}

void HeaderWidget::draw(DisplayManager& display)
{
    titleLabel.draw(display);
    sdIcon.draw(display);
    wifiIcon.draw(display);
}