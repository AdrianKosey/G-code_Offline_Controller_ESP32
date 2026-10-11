#include "info_modal_widget.h"
#include "../../../i18n/translations.h"

namespace
{
    constexpr int16_t DIALOG_WIDTH = 240;
    constexpr int16_t MIN_DIALOG_HEIGHT = 100;
    constexpr int16_t MAX_DIALOG_HEIGHT = 200;
    constexpr int16_t BUTTON_AREA_HEIGHT = 40;
    constexpr int16_t TEXT_PADDING_TOP = 14;
    constexpr int16_t BUTTON_WIDTH = 100;

    Rect centeredRect(const Rect& screenBounds, int16_t w, int16_t h)
    {
        return Rect{
            (int16_t)(screenBounds.x + (screenBounds.width - w) / 2),
            (int16_t)(screenBounds.y + (screenBounds.height - h) / 2),
            w, h
        };
    }
}

InfoModalWidget::InfoModalWidget(const Rect& screenBounds)
    : screenBounds(screenBounds),
      panel(Rect{0, 0, DIALOG_WIDTH, MIN_DIALOG_HEIGHT}, Theme::Panel, 10),
      messageArea(Rect{0, 0, DIALOG_WIDTH - 20, 40}, Theme::Text, 2, Theme::Panel),
      okButton(Rect{0, 0, BUTTON_WIDTH, 28}, tr(StringId::Modal_Ok))
{
    okButton.setOnPress([this]() {
        visible = false;
        if (onClose) onClose();
    });
}

void InfoModalWidget::setOnClose(CloseCallback callback) { onClose = callback; }

void InfoModalWidget::show(DisplayManager& display, const String& message)
{
    int16_t textAreaWidth = DIALOG_WIDTH - 20;

    messageArea.setBounds(Rect{0, 0, textAreaWidth, 1000});
    messageArea.setText(message);
    int16_t contentHeight = messageArea.measure(display);

    int16_t availableTextHeight = min(contentHeight, (int16_t)(MAX_DIALOG_HEIGHT - BUTTON_AREA_HEIGHT - TEXT_PADDING_TOP));
    int16_t dialogHeight = max(MIN_DIALOG_HEIGHT, (int16_t)(TEXT_PADDING_TOP + availableTextHeight + BUTTON_AREA_HEIGHT));
    dialogHeight = min(dialogHeight, MAX_DIALOG_HEIGHT);

    Rect dialog = centeredRect(screenBounds, DIALOG_WIDTH, dialogHeight);

    panel.setBounds(dialog);

    messageArea.setBounds(Rect{
        (int16_t)(dialog.x + 10),
        (int16_t)(dialog.y + TEXT_PADDING_TOP),
        textAreaWidth,
        (int16_t)(dialogHeight - TEXT_PADDING_TOP - BUTTON_AREA_HEIGHT)
    });
    messageArea.setText(message);

    okButton.setBounds(Rect{
        (int16_t)(dialog.x + (dialog.width - BUTTON_WIDTH) / 2),
        (int16_t)(dialog.y + dialogHeight - 34),
        BUTTON_WIDTH, 28
    });

    visible = true;
    dirty = true;
}

void InfoModalWidget::hide() { visible = false; }
bool InfoModalWidget::isVisible() const { return visible; }

void InfoModalWidget::draw(DisplayManager& display)
{
    if (!visible)
        return;

    if (dirty)
    {
        display.fillRect(screenBounds.x, screenBounds.y, screenBounds.width, screenBounds.height, Theme::Border);
        dirty = false;

        panel.invalidate();
        messageArea.invalidate();
        okButton.invalidate();
    }

    panel.draw(display);
    messageArea.draw(display);
    okButton.draw(display);
}

bool InfoModalWidget::handleTouch(const TouchEvent& event)
{
    if (!visible)
        return false;

    messageArea.handleTouch(event);
    okButton.handleTouch(event);

    return true;
}