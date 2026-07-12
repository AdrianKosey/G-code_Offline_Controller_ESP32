#include "keyboard_modal_widget.h"

namespace
{
    const char* LETTER_ROWS[3] = { "qwertyuiop", "asdfghjkl", "zxcvbnm" };
    const char* SYMBOL_ROWS[3] = { "1234567890", "-/:;()$&@\"", ".,?!'#%^*+" };
}

KeyboardModalWidget::KeyboardModalWidget(const Rect& screenBounds)
    : screenBounds(screenBounds),
      panel(Rect{screenBounds.x, (int16_t)(screenBounds.y+60), screenBounds.width, (int16_t)(screenBounds.height-60)}, Theme::Panel, 0),
      titleLabel(Rect{(int16_t)(screenBounds.x + 10), (int16_t)(screenBounds.y + 66), 220, 18}, "", Theme::TextSecondary, 1, Theme::Panel, false),
      inputLabel(Rect{(int16_t)(screenBounds.x + 10), (int16_t)(screenBounds.y + 84), (int16_t)(screenBounds.width - 20), 24}, "", Theme::Text, 2, Theme::Panel, true),
      shiftButton(Rect{(int16_t)(screenBounds.x + 4), (int16_t)(screenBounds.y + 192), 40, 26}, "^"),
      modeToggleButton(Rect{(int16_t)(screenBounds.x + 48), (int16_t)(screenBounds.y + 192), 44, 26}, "?123"),
      spaceButton(Rect{(int16_t)(screenBounds.x + 96), (int16_t)(screenBounds.y + 192), 120, 26}, "espacio"),
      backspaceButton(Rect{(int16_t)(screenBounds.x + screenBounds.width - 40), (int16_t)(screenBounds.y + 192), 36, 26}, "<-"),
      cancelButton(Rect{(int16_t)(screenBounds.x + 4), (int16_t)(screenBounds.y + 222), 60, 26}, "Cancelar"),
      submitButton(Rect{(int16_t)(screenBounds.x + screenBounds.width - 64), (int16_t)(screenBounds.y + 222), 60, 26}, "OK")
{
    shiftButton.setOnPress([this]() {
        shiftActive = !shiftActive;
        shiftButton.setSelected(shiftActive);
        refreshKeyLabels();
    });

    modeToggleButton.setOnPress([this]() { toggleMode(); });

    spaceButton.setOnPress([this]() { appendChar(' '); });

    backspaceButton.setOnPress([this]() {
        if (inputText.length() > 0)
        {
            inputText.remove(inputText.length() - 1);
            inputLabel.setText(inputText);
        }
    });

    cancelButton.setOnPress([this]() {
        hide();
        if (onCancel) onCancel();
    });

    submitButton.setOnPress([this]() {
        String result = inputText;
        hide();
        if (onSubmit) onSubmit(result);
    });

    buildLayout();
}

void KeyboardModalWidget::buildLayout()
{
    const char** rows = (mode == KeyboardMode::Letters) ? LETTER_ROWS : SYMBOL_ROWS;

    int16_t startY = screenBounds.y + 108;
    int16_t keyH = 26, gap = 2;
    int16_t gridWidth = screenBounds.width - 8;
    int16_t keyW = gridWidth / 10;

    keyCount = 0;

    for (uint8_t row = 0; row < 3; row++)
    {
        const char* rowChars = rows[row];
        int16_t rowY = startY + row * (keyH + gap);
        int16_t x = screenBounds.x + 4;

        while (*rowChars && keyCount < MAX_KEYS)
        {
            keyChars[keyCount] = *rowChars;
            keys[keyCount] = ButtonWidget(Rect{x, rowY, (int16_t)(keyW - gap), keyH}, String(*rowChars));

            uint8_t idx = keyCount;
            keys[keyCount].setOnPress([this, idx]() { appendChar(keyChars[idx]); });

            keyCount++;
            x += keyW;
            rowChars++;
        }
    }

    modeToggleButton.setText(mode == KeyboardMode::Letters ? "?123" : "ABC");
    shiftButton.setSelected(false);
    shiftActive = false;

    refreshKeyLabels();
}

void KeyboardModalWidget::toggleMode()
{
    mode = (mode == KeyboardMode::Letters) ? KeyboardMode::Symbols : KeyboardMode::Letters;
    buildLayout();

    dirty = true;
}

void KeyboardModalWidget::setOnSubmit(SubmitCallback callback) { onSubmit = callback; }
void KeyboardModalWidget::setOnCancel(CancelCallback callback) { onCancel = callback; }

void KeyboardModalWidget::appendChar(char c)
{
    if (inputText.length() >= 63)
        return;

    bool isLetter = (c >= 'a' && c <= 'z');
    inputText += (shiftActive && isLetter) ? (char)toupper(c) : c;

    inputLabel.setText(inputText);
}

void KeyboardModalWidget::refreshKeyLabels()
{
    for (uint8_t i = 0; i < keyCount; i++)
    {
        char c = keyChars[i];
        bool isLetter = (c >= 'a' && c <= 'z');

        char displayChar = (shiftActive && isLetter) ? toupper(c) : c;
        keys[i].setText(String(displayChar));
    }
}

void KeyboardModalWidget::show(const String& title)
{
    titleLabel.setText(title);
    inputText = "";
    inputLabel.setText("");

    mode = KeyboardMode::Letters;
    buildLayout();

    visible = true;
    dirty = true;

    panel.invalidate();
    titleLabel.invalidate();
    inputLabel.invalidate();
    shiftButton.invalidate();
    modeToggleButton.invalidate();
    spaceButton.invalidate();
    backspaceButton.invalidate();
    cancelButton.invalidate();
    submitButton.invalidate();

    for (uint8_t i = 0; i < keyCount; i++)
        keys[i].invalidate();
}

void KeyboardModalWidget::hide() { visible = false; }
bool KeyboardModalWidget::isVisible() const { return visible; }

void KeyboardModalWidget::draw(DisplayManager& display)
{
    if (!visible)
        return;

    if (dirty)
    {
        display.fillRect(screenBounds.x, screenBounds.y, screenBounds.width, screenBounds.height, Theme::Border);
        dirty = false;

        panel.invalidate();
        titleLabel.invalidate();
        inputLabel.invalidate();
        shiftButton.invalidate();
        modeToggleButton.invalidate();
        spaceButton.invalidate();
        backspaceButton.invalidate();
        cancelButton.invalidate();
        submitButton.invalidate();

        for (uint8_t i = 0; i < keyCount; i++)
            keys[i].invalidate();
    }

    panel.draw(display);
    titleLabel.draw(display);
    inputLabel.draw(display);
    shiftButton.draw(display);
    modeToggleButton.draw(display);
    spaceButton.draw(display);
    backspaceButton.draw(display);
    cancelButton.draw(display);
    submitButton.draw(display);

    for (uint8_t i = 0; i < keyCount; i++)
        keys[i].draw(display);
}

bool KeyboardModalWidget::handleTouch(const TouchEvent& event)
{
    if (!visible)
        return false;

    shiftButton.handleTouch(event);
    modeToggleButton.handleTouch(event);
    spaceButton.handleTouch(event);
    backspaceButton.handleTouch(event);
    cancelButton.handleTouch(event);
    submitButton.handleTouch(event);

    for (uint8_t i = 0; i < keyCount; i++)
        keys[i].handleTouch(event);

    return true;
}