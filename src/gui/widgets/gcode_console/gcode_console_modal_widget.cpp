#include "gcode_console_modal_widget.h"
#include "../../icons.h"
#include "../../../i18n/translations.h"
namespace
{
    // Casi toda la pantalla, con margen chico - este modal necesita mucho espacio para el log
    Rect panelBounds(const Rect& screenBounds)
    {
        return Rect{
            (int16_t)(screenBounds.x + 6),
            (int16_t)(screenBounds.y + 6),
            (int16_t)(screenBounds.width - 12),
            (int16_t)(screenBounds.height - 12)
        };
    }
}

GcodeConsoleModalWidget::GcodeConsoleModalWidget(const Rect& screenBounds, GrblController& grblController)
    : screenBounds(screenBounds), grbl(grblController),
      panel(panelBounds(screenBounds), Theme::Panel, 10),
      titleLabel(Rect{(int16_t)(screenBounds.x + 16), (int16_t)(screenBounds.y + 12), 200, 20}, tr(StringId::Console_Title), Theme::Text, 2, Theme::Panel, false),
      closeButton(
          Rect{(int16_t)(screenBounds.x + screenBounds.width - 40), (int16_t)(screenBounds.y + 10), 26, 24},
          Icons::Back, Icons::HEADER_WIDTH, Icons::HEADER_HEIGHT,
          Theme::Panel, Theme::Text, false),
      consoleLog(Rect{(int16_t)(screenBounds.x + 10), (int16_t)(screenBounds.y + 40), (int16_t)(screenBounds.width - 20), (int16_t)(screenBounds.height - 82)}, 16),
      commandInputRow(Rect{(int16_t)(screenBounds.x + 10), (int16_t)(screenBounds.y + screenBounds.height - 39), (int16_t)(screenBounds.width - 20), 32}, tr(StringId::Console_Command)),
      commandKeyboard(Rect{screenBounds.x, screenBounds.y, screenBounds.width, screenBounds.height})
{
    closeButton.setOnPress([this]() { hide(); });

    commandInputRow.setOnPress([this]() {
        commandKeyboard.show(tr(StringId::Console_Command));
    });

    commandKeyboard.setOnSubmit([this](const String& command) {
        if (command.length() > 0)
            grbl.sendRaw(command);
        dirty = true; // Redraw the modal to ensure the keyboard is cleared
    });

    commandKeyboard.setOnCancel([this]() {
        commandKeyboard.hide();
        dirty = true; // Redraw the modal to ensure the keyboard is cleared
    });
}

void GcodeConsoleModalWidget::show(DisplayManager& display)
{
    visible = true;
    dirty = true;

    panel.invalidate();
    titleLabel.invalidate();
    closeButton.invalidate();
    consoleLog.invalidate();
    commandInputRow.invalidate();
}

void GcodeConsoleModalWidget::hide() { visible = false; }
bool GcodeConsoleModalWidget::isVisible() const { return visible; }

void GcodeConsoleModalWidget::update()
{
    if (!visible)
        return;

    consoleLog.syncFrom(grbl);
}

void GcodeConsoleModalWidget::draw(DisplayManager& display)
{
    if (!visible)
        return;

    if (dirty)
    {
        display.fillRect(screenBounds.x, screenBounds.y, screenBounds.width, screenBounds.height, Theme::Border);
        dirty = false;

        panel.invalidate();
        titleLabel.invalidate();
        closeButton.invalidate();
        consoleLog.invalidate();
        commandInputRow.invalidate();
    }

    panel.draw(display);
    titleLabel.draw(display);
    closeButton.draw(display);
    consoleLog.draw(display);
    commandInputRow.draw(display);

    // Keyboard 
    commandKeyboard.draw(display);
}

bool GcodeConsoleModalWidget::handleTouch(const TouchEvent& event)
{
    if (!visible)
        return false;

    // Keyboard has priority if visible
    if (commandKeyboard.isVisible())
        return commandKeyboard.handleTouch(event);

    closeButton.handleTouch(event);
    consoleLog.handleTouch(event);
    commandInputRow.handleTouch(event);

    return true;
}