#include "confirm_modal_widget.h"
#include "../../../i18n/translations.h"
namespace
{
    Rect dialogBounds(const Rect& screenBounds)
    {
        constexpr int16_t w = 220;
        constexpr int16_t h = 110;

        return Rect{
            (int16_t)(screenBounds.x + (screenBounds.width - w) / 2),
            (int16_t)(screenBounds.y + (screenBounds.height - h) / 2),
            w, h
        };
    }

    Rect messageLabelBounds(const Rect& screenBounds)
    {
        Rect d = dialogBounds(screenBounds);
        return Rect{ (int16_t)(d.x + 12), (int16_t)(d.y + 14), (int16_t)(d.width - 24), 40 };
    }

    Rect yesButtonBounds(const Rect& screenBounds)
    {
        Rect d = dialogBounds(screenBounds);
        return Rect{ (int16_t)(d.x + 20), (int16_t)(d.y + d.height - 40), 80, 28 };
    }

    Rect noButtonBounds(const Rect& screenBounds)
    {
        Rect d = dialogBounds(screenBounds);
        return Rect{ (int16_t)(d.x + d.width - 100), (int16_t)(d.y + d.height - 40), 80, 28 };
    }
}

ConfirmModalWidget::ConfirmModalWidget(const Rect& screenBounds)
    : screenBounds(screenBounds),
      panel(dialogBounds(screenBounds), Theme::Panel, 10),
      messageLabel(messageLabelBounds(screenBounds), "", Theme::Text, 2, Theme::Panel),
      yesButton(yesButtonBounds(screenBounds), tr(StringId::App_Yes)),
      noButton(noButtonBounds(screenBounds), tr(StringId::App_No))
{
    yesButton.setOnPress([this]() {
        visible = false;
        if (onConfirm) onConfirm();
    });

    noButton.setOnPress([this]() {
        visible = false;
        if (onCancel) onCancel();
    });
}

void ConfirmModalWidget::setOnConfirm(ConfirmCallback callback) { onConfirm = callback; }
void ConfirmModalWidget::setOnCancel(ConfirmCallback callback) { onCancel = callback; }

void ConfirmModalWidget::show(const String& message)
{
    messageLabel.setText(message);
    visible = true;
    dirty = true;

    panel.invalidate();
    messageLabel.invalidate();
    yesButton.invalidate();
    noButton.invalidate();
}

void ConfirmModalWidget::hide() { visible = false; }
bool ConfirmModalWidget::isVisible() const { return visible; }

void ConfirmModalWidget::draw(DisplayManager& display)
{
    if (!visible || !dirty)
        return;

    display.fillRect(screenBounds.x, screenBounds.y, screenBounds.width, screenBounds.height, Theme::Border);

    panel.draw(display);
    messageLabel.draw(display);
    yesButton.draw(display);
    noButton.draw(display);

    dirty = false;
}

bool ConfirmModalWidget::handleTouch(const TouchEvent& event)
{
    if (!visible)
        return false;

    yesButton.handleTouch(event);
    noButton.handleTouch(event);

    return true;
}