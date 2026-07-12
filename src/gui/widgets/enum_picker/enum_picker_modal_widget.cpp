#include "enum_picker_modal_widget.h"

EnumPickerModalWidget::EnumPickerModalWidget(const Rect& screenBounds)
    : screenBounds(screenBounds),
      panel(Rect{(int16_t)(screenBounds.x + 20), (int16_t)(screenBounds.y + 40), (int16_t)(screenBounds.width - 40), (int16_t)(screenBounds.height - 80)}, Theme::Panel, 10),
      titleLabel(Rect{(int16_t)(screenBounds.x + 32), (int16_t)(screenBounds.y + 50), (int16_t)(screenBounds.width - 64), 20}, "", Theme::Text, 2, Theme::Panel, false)
{}

void EnumPickerModalWidget::setOnSubmit(SubmitCallback callback) { onSubmit = callback; }

void EnumPickerModalWidget::show(const String& title, const char** labels, uint8_t count, uint8_t currentIndex)
{
    titleLabel.setText(title);

    optionCount = min(count, MAX_OPTIONS);

    int16_t startY = screenBounds.y + 80;

    for (uint8_t i = 0; i < optionCount; i++)
    {
        optionButtons[i] = ButtonWidget(
            Rect{(int16_t)(screenBounds.x + 32), (int16_t)(startY + i * 34), (int16_t)(screenBounds.width - 64), 28},
            labels[i]);

        optionButtons[i].setSelected(i == currentIndex);

        uint8_t idx = i;
        optionButtons[i].setOnPress([this, idx]() {
            hide();
            if (onSubmit) onSubmit(idx);
        });
    }

    visible = true;
    dirty = true;
}

void EnumPickerModalWidget::hide() { visible = false; }
bool EnumPickerModalWidget::isVisible() const { return visible; }

void EnumPickerModalWidget::draw(DisplayManager& display)
{
    if (!visible)
        return;

    if (dirty)
    {
        display.fillRect(screenBounds.x, screenBounds.y, screenBounds.width, screenBounds.height, Theme::Border);
        dirty = false;

        panel.invalidate();
        titleLabel.invalidate();
        for (uint8_t i = 0; i < optionCount; i++) optionButtons[i].invalidate();
    }

    panel.draw(display);
    titleLabel.draw(display);

    for (uint8_t i = 0; i < optionCount; i++)
        optionButtons[i].draw(display);
}

bool EnumPickerModalWidget::handleTouch(const TouchEvent& event)
{
    if (!visible)
        return false;

    for (uint8_t i = 0; i < optionCount; i++)
        optionButtons[i].handleTouch(event);

    return true;
}