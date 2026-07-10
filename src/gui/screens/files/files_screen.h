#pragma once
#include "../../core/screen.h"
#include "../../theme.h"

class FilesScreen : public IScreen
{
public:
    void draw(DisplayManager& display) override
    {
        display.fillRect(60, 0, display.width() - 60, display.height(), Theme::Background);
        display.drawText("Archivos (placeholder)", 80, 20, Theme::Text);
    }
};