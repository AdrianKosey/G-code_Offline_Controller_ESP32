#pragma once

#include "../../display/display_manager.h"
#include "../../touch/touch_event.h"

class IScreen
{
public:
    virtual ~IScreen() = default;

    virtual void onEnter() {}
    virtual void onExit() {}

    virtual void draw(DisplayManager& display) = 0;

    virtual void update() {}

    virtual bool handleTouch(const TouchEvent&)
    {
        return false;
    }
};