#include "confirm_modal_widget.h"
#include "../../../i18n/translations.h"
namespace
{
    constexpr int16_t DIALOG_WIDTH = 240;
    constexpr int16_t MIN_DIALOG_HEIGHT = 110;
    constexpr int16_t MAX_DIALOG_HEIGHT = 200;
    constexpr int16_t BUTTON_AREA_HEIGHT = 40;
    constexpr int16_t TEXT_PADDING_TOP = 14;

    Rect centeredRect(const Rect& screenBounds, int16_t w, int16_t h)
    {
        return Rect{
            (int16_t)(screenBounds.x + (screenBounds.width - w) / 2),
            (int16_t)(screenBounds.y + (screenBounds.height - h) / 2),
            w, h
        };
    }
}

ConfirmModalWidget::ConfirmModalWidget(const Rect& screenBounds)
    : screenBounds(screenBounds),
      panel(Rect{0, 0, DIALOG_WIDTH, MIN_DIALOG_HEIGHT}, Theme::Panel, 10),
      messageArea(Rect{0, 0, DIALOG_WIDTH - 20, 40}, Theme::Text, 2, Theme::Panel),
      yesButton(Rect{0, 0, 80, 28}, tr(StringId::App_Yes)),
      noButton(Rect{0, 0, 80, 28}, tr(StringId::App_No))
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

void ConfirmModalWidget::show(DisplayManager& display, const String& message)
{
    // 1. Mide cuanto texto envuelto necesita, con el ancho fijo del dialogo
    int16_t textAreaWidth = DIALOG_WIDTH - 20;
    messageArea.setBounds(Rect{0, 0, textAreaWidth, 1000}); // alto temporal grande solo para medir
    messageArea.setText(message);
    int16_t contentHeight = messageArea.measure(display);

    // 2. Calcula el alto real del dialogo, respetando min/max
    int16_t availableTextHeight = min(contentHeight, (int16_t)(MAX_DIALOG_HEIGHT - BUTTON_AREA_HEIGHT - TEXT_PADDING_TOP));
    int16_t dialogHeight = max(MIN_DIALOG_HEIGHT, (int16_t)(TEXT_PADDING_TOP + availableTextHeight + BUTTON_AREA_HEIGHT));
    dialogHeight = min(dialogHeight, MAX_DIALOG_HEIGHT);

    Rect dialog = centeredRect(screenBounds, DIALOG_WIDTH, dialogHeight);

    // 3. Reposiciona todo segun el tamano final
    panel.setBounds(dialog);

    Rect textBounds{
        (int16_t)(dialog.x + 10),
        (int16_t)(dialog.y + TEXT_PADDING_TOP),
        textAreaWidth,
        (int16_t)(dialogHeight - TEXT_PADDING_TOP - BUTTON_AREA_HEIGHT)
    };
    messageArea.setBounds(textBounds);
    messageArea.setText(message); // se vuelve a envolver, ahora con el alto real (por si cambia el scroll)

    yesButton.setBounds(Rect{(int16_t)(dialog.x + 20), (int16_t)(dialog.y + dialogHeight - 34), 80, 28});
    noButton.setBounds(Rect{(int16_t)(dialog.x + dialog.width - 100), (int16_t)(dialog.y + dialogHeight - 34), 80, 28});

    visible = true;
    dirty = true;

    panel.invalidate();
    messageArea.invalidate();
    yesButton.invalidate();
    noButton.invalidate();
}

void ConfirmModalWidget::hide() { visible = false; }
bool ConfirmModalWidget::isVisible() const { return visible; }

void ConfirmModalWidget::draw(DisplayManager& display)
{
    if (!visible)
        return;

    if (dirty)
    {
        display.fillRect(screenBounds.x, screenBounds.y, screenBounds.width, screenBounds.height, Theme::Border);
        dirty = false;

        panel.invalidate();
        messageArea.invalidate();
        yesButton.invalidate();
        noButton.invalidate();
    }

    panel.draw(display);
    messageArea.draw(display);
    yesButton.draw(display);
    noButton.draw(display);
}

bool ConfirmModalWidget::handleTouch(const TouchEvent& event)
{
    if (!visible)
        return false;

    messageArea.handleTouch(event);
    yesButton.handleTouch(event);
    noButton.handleTouch(event);

    return true;
}