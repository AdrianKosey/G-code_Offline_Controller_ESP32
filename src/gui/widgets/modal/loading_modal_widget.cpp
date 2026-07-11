#include "loading_modal_widget.h"

namespace
{
    Rect dialogBounds(const Rect& screenBounds)
    {
        constexpr int16_t w = 180;
        constexpr int16_t h = 70;

        return Rect{
            (int16_t)(screenBounds.x + (screenBounds.width - w) / 2),
            (int16_t)(screenBounds.y + (screenBounds.height - h) / 2),
            w, h
        };
    }

    Rect messageLabelBounds(const Rect& screenBounds)
    {
        Rect d = dialogBounds(screenBounds);
        return Rect{ (int16_t)(d.x + 12), (int16_t)(d.y + 22), (int16_t)(d.width - 24), 26 };
    }
}

LoadingModalWidget::LoadingModalWidget(const Rect& screenBounds)
    : screenBounds(screenBounds),
      panel(dialogBounds(screenBounds), Theme::Panel, 10),
      messageLabel(messageLabelBounds(screenBounds), "", Theme::Text, 2, Theme::Panel)
{}

void LoadingModalWidget::show(const String& message)
{
    messageLabel.setText(message);
    visible = true;
    dirty = true;

    panel.invalidate();
    messageLabel.invalidate();
}

void LoadingModalWidget::hide() { visible = false; }
bool LoadingModalWidget::isVisible() const { return visible; }

void LoadingModalWidget::draw(DisplayManager& display)
{
    if (!visible || !dirty)
        return;

    display.fillRect(screenBounds.x, screenBounds.y, screenBounds.width, screenBounds.height, Theme::Border);

    panel.draw(display);
    messageLabel.draw(display);

    dirty = false;
}

bool LoadingModalWidget::handleTouch(const TouchEvent& event)
{
    return visible;
}