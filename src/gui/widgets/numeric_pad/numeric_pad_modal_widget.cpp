#include "numeric_pad_modal_widget.h"

NumericPadModalWidget::NumericPadModalWidget(const Rect& screenBounds)
    : screenBounds(screenBounds),
      panel(Rect{screenBounds.x, screenBounds.y, screenBounds.width, screenBounds.height}, Theme::Panel, 0),
      
      // Title and Input aligned proportionally above
      titleLabel(Rect{(int16_t)(screenBounds.x + 8), (int16_t)(screenBounds.y + 8), (int16_t)(screenBounds.width - 16), 18}, "", Theme::TextSecondary, 1, Theme::Panel, false),
      inputLabel(Rect{(int16_t)(screenBounds.x + 8), (int16_t)(screenBounds.y + 30), (int16_t)(screenBounds.width - 16), 26}, "", Theme::Text, 2, Theme::Panel, false),
      
      // Temporary initializations (dynamically recalculated in the constructor body)
      dotButton(Rect{0, 0, 0, 0}, "."),
      signButton(Rect{0, 0, 0, 0}, "+/-"),
      backspaceButton(Rect{0, 0, 0, 0}, "<-"),
      cancelButton(Rect{0, 0, 0, 0}, "Cancelar"),
      submitButton(Rect{0, 0, 0, 0}, "OK")
{
    // ---- BUTTON GRID CONFIGURATION ----
    int16_t paddingX = 8;
    int16_t gap = 4; // Space between buttons
    int16_t keyW = (screenBounds.width - (2 * paddingX) - (2 * gap)) / 3;
    int16_t keyH = 30;
    int16_t startY = screenBounds.y + 64; // Starts just below the input display

    // X coordinates of the 3 columns
    int16_t col0_X = screenBounds.x + paddingX;
    int16_t col1_X = screenBounds.x + paddingX + keyW + gap;
    int16_t col2_X = screenBounds.x + paddingX + 2 * (keyW + gap);

    // ---- ROWS 0, 1, 2: Digits 1 to 9 ----
    for (uint8_t i = 0; i < 9; i++)
    {
        int16_t col = i % 3;
        int16_t row = i / 3;
        int16_t btnX = screenBounds.x + paddingX + col * (keyW + gap);
        int16_t btnY = startY + row * (keyH + gap);

        digitButtons[i] = ButtonWidget(Rect{btnX, btnY, keyW, keyH}, String(i + 1));

        uint8_t digit = i + 1;
        digitButtons[i].setOnPress([this, digit]() { appendChar('0' + digit); });
    }

    // ---- ROW 3: [ +/- ] [ 0 ] [ . ] ----
    int16_t row3_Y = startY + 3 * (keyH + gap);

    signButton = ButtonWidget(Rect{col0_X, row3_Y, keyW, keyH}, "+/-");
    
    digitButtons[9] = ButtonWidget(Rect{col1_X, row3_Y, keyW, keyH}, "0");
    
    dotButton = ButtonWidget(Rect{col2_X, row3_Y, keyW, keyH}, ".");

    // ---- ROW 4: [ Cancel ] [ <- ] [ OK ] ----
    int16_t row4_Y = startY + 4 * (keyH + gap);

    cancelButton    = ButtonWidget(Rect{col0_X, row4_Y, keyW, keyH}, "Cancelar");
    backspaceButton = ButtonWidget(Rect{col1_X, row4_Y, keyW, keyH}, "<-");
    submitButton    = ButtonWidget(Rect{col2_X, row4_Y, keyW, keyH}, "OK");

    // ---- ACTION CONFIGURATION (Logic intact) ----
    digitButtons[9].setOnPress([this]() { appendChar('0'); });
    dotButton.setOnPress([this]() { appendChar('.'); });

    signButton.setOnPress([this]() {
        if (inputText.startsWith("-"))
            inputText.remove(0, 1);
        else
            inputText = "-" + inputText;
        inputLabel.setText(inputText);
    });

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
        float result = inputText.toFloat();
        hide();
        if (onSubmit) onSubmit(result);
    });
}

void NumericPadModalWidget::setOnSubmit(SubmitCallback callback) { onSubmit = callback; }
void NumericPadModalWidget::setOnCancel(CancelCallback callback) { onCancel = callback; }

void NumericPadModalWidget::appendChar(char c)
{
    if (inputText.length() >= 12)
        return;

    if (c == '.' && inputText.indexOf('.') >= 0)
        return; 

    inputText += c;
    inputLabel.setText(inputText);
}

void NumericPadModalWidget::show(const String& title, float currentValue)
{
    titleLabel.setText(title);
    inputText = String(currentValue, 3);
    inputLabel.setText(inputText);

    visible = true;
    dirty = true;
}

void NumericPadModalWidget::hide() { visible = false; }
bool NumericPadModalWidget::isVisible() const { return visible; }

void NumericPadModalWidget::draw(DisplayManager& display)
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
        for (uint8_t i = 0; i < 10; i++) digitButtons[i].invalidate();
        dotButton.invalidate();
        signButton.invalidate();
        backspaceButton.invalidate();
        cancelButton.invalidate();
        submitButton.invalidate();
    }

    panel.draw(display);
    titleLabel.draw(display);
    inputLabel.draw(display);

    for (uint8_t i = 0; i < 10; i++)
        digitButtons[i].draw(display);

    dotButton.draw(display);
    signButton.draw(display);
    backspaceButton.draw(display);
    cancelButton.draw(display);
    submitButton.draw(display);
}

bool NumericPadModalWidget::handleTouch(const TouchEvent& event)
{
    if (!visible)
        return false;

    for (uint8_t i = 0; i < 10; i++)
        digitButtons[i].handleTouch(event);

    dotButton.handleTouch(event);
    signButton.handleTouch(event);
    backspaceButton.handleTouch(event);
    cancelButton.handleTouch(event);
    submitButton.handleTouch(event);

    return true;
}