#include "keyboard_modal_widget.h"
#include <cstring>

namespace
{
    const char* LETTER_ROWS[3] = { "qwertyuiop", "asdfghjkl", "zxcvbnm" };
    const char* SYMBOL_ROWS[3] = { "1234567890", "-/:;()$&@\"", ".,?!'#%^*+" };
}

KeyboardModalWidget::KeyboardModalWidget(const Rect& screenBounds)
    : screenBounds(screenBounds),
      panel(Rect{screenBounds.x, (int16_t)(screenBounds.y + 60), screenBounds.width, (int16_t)(screenBounds.height - 60)}, Theme::Panel, 0),
      
      // Proportional adjustment for the upper displays of the modal
      titleLabel(Rect{(int16_t)(screenBounds.x + 8), (int16_t)(screenBounds.y + 64), (int16_t)(screenBounds.width - 16), 14}, "", Theme::TextSecondary, 1, Theme::Panel, false),
      inputLabel(Rect{(int16_t)(screenBounds.x + 8), (int16_t)(screenBounds.y + 80), (int16_t)(screenBounds.width - 16), 22}, "", Theme::Text, 2, Theme::Panel, true),
      

      shiftButton(Rect{0, 0, 0, 0}, "^"),
      modeToggleButton(Rect{0, 0, 0, 0}, "?123"),
      spaceButton(Rect{0, 0, 0, 0}, "space"),
      backspaceButton(Rect{0, 0, 0, 0}, "<-"),
      cancelButton(Rect{0, 0, 0, 0}, "cancel"),
      submitButton(Rect{0, 0, 0, 0}, "OK")
{
    buildLayout();
}

void KeyboardModalWidget::buildLayout()
{
    const char** rows = (mode == KeyboardMode::Letters) ? LETTER_ROWS : SYMBOL_ROWS;

    // ---- HORIZONTAL GEOMETRY (Based on a maximum row of 10 keys) ----
    int16_t maxKeys = 10;
    int16_t gap = 2; // Spacing between buttons
    int16_t keyW = (screenBounds.width - 8 - ((maxKeys - 1) * gap)) / maxKeys;
    int16_t totalGridW = maxKeys * keyW + (maxKeys - 1) * gap;
    int16_t paddingX = (screenBounds.width - totalGridW) / 2; //  centering of the keyboard block

    // ---- VERTICAL GEOMETRY (Reduced to 24px high so that the 5 rows fit in 180px) ----
    int16_t panelY = screenBounds.y + 60;
    int16_t keyH = 24; 
    int16_t startY = panelY + 46; // Starts just below the input label

    keyCount = 0;

    // ---- ROWS 0, 1, 2: Character Matrix ----
    for (uint8_t row = 0; row < 3; row++)
    {
        const char* rowChars = rows[row];
        int16_t rowY = startY + row * (keyH + gap);
        
        // CENTERING CALCULATION FOR SHORT ROWS (e.g., 'asdfghjkl' or 'zxcvbnm')
        int16_t numKeysInRow = strlen(rowChars);
        int16_t rowWidth = numKeysInRow * keyW + (numKeysInRow - 1) * gap;
        int16_t x = screenBounds.x + (screenBounds.width - rowWidth) / 2;

        while (*rowChars && keyCount < MAX_KEYS)
        {
            keyChars[keyCount] = *rowChars;
            keys[keyCount] = ButtonWidget(Rect{x, rowY, keyW, keyH}, String(*rowChars));

            uint8_t idx = keyCount;
            keys[keyCount].setOnPress([this, idx]() { appendChar(keyChars[idx]); });

            keyCount++;
            x += keyW + gap;
            rowChars++;
        }
    }

    // ---- ROW 3: [ ^ ] [ ?123 ] [ Space ] [ <- ] ----
    int16_t row3_Y = startY + 3 * (keyH + gap);
    int16_t sideBtnW = (keyW * 15) / 10;
    int16_t totalActionRowW = screenBounds.width - (2 * paddingX);
    int16_t spaceBtnW = totalActionRowW - (3 * sideBtnW) - (3 * gap);

    int16_t currentX = screenBounds.x + paddingX;
    
    shiftButton = ButtonWidget(Rect{currentX, row3_Y, sideBtnW, keyH}, "^");
    currentX += sideBtnW + gap;

    modeToggleButton = ButtonWidget(Rect{currentX, row3_Y, sideBtnW, keyH}, mode == KeyboardMode::Letters ? "?123" : "ABC");
    currentX += sideBtnW + gap;

    spaceButton = ButtonWidget(Rect{currentX, row3_Y, spaceBtnW, keyH}, "          -------");
    currentX += spaceBtnW + gap;

    backspaceButton = ButtonWidget(Rect{currentX, row3_Y, sideBtnW, keyH}, "<-");

    // ---- ROW 4: [ Cancel ] [ OK ] ----
    int16_t row4_Y = startY + 4 * (keyH + gap);
    int16_t bottomBtnW = (totalActionRowW - gap) / 2; 

    cancelButton = ButtonWidget(Rect{(int16_t)(screenBounds.x + paddingX), row4_Y, bottomBtnW, keyH}, "Cancelar");
    submitButton = ButtonWidget(Rect{(int16_t)(screenBounds.x + paddingX + bottomBtnW + gap), row4_Y, bottomBtnW, keyH}, "OK");


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

    shiftButton.setSelected(shiftActive);
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