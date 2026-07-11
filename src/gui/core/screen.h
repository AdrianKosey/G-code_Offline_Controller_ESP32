#pragma once

#include <vector>

#include "../../display/display_manager.h"
#include "../../touch/touch_event.h"
#include "widget.h"

class IScreen
{
public:
    virtual ~IScreen() = default;

    virtual void onEnter()
    {
        for (Widget* widget : widgets)
            widget->invalidate();
    }

    virtual void onExit() {}

    virtual void draw(DisplayManager& display)
    {
        for (Widget* widget : widgets)
            widget->draw(display);
    }

    virtual void update()
    {
        for (Widget* widget : widgets)
            widget->update();
    }

    virtual bool handleTouch(const TouchEvent& event)
    {
        for (Widget* widget : widgets)
        {
            if (widget->handleTouch(event))
                return true;
        }

        return false;
    }

protected:
    std::vector<Widget*> widgets;
};