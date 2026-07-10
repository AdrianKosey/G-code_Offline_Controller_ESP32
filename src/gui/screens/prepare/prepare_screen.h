#pragma once
#include "../../core/screen.h"
#include "../../theme.h"

class PrepareScreen : public IScreen
{
public:
    void draw(DisplayManager& display) override
    {
        display.fillRect(60, 0, display.width() - 60, display.height(), Theme::Background);
        display.drawText("Preparar (placeholder)", 80, 20, Theme::Text);
    }
};