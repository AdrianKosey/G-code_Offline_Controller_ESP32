#pragma once

#include <Arduino.h>

#include "rect.h"

#include "../../display/display_manager.h"
#include "../../touch/touch_event.h"

class Widget
{
public:
    Widget() : bounds{0, 0, 0, 0} {}

    Widget(const Rect& bounds)
        : bounds(bounds)
    {
    }

    virtual ~Widget() = default;

    virtual void draw(DisplayManager& display) = 0;

    virtual void update()
    {
    }

    virtual bool handleTouch(const TouchEvent&)
    {
        return false;
    }

    void invalidate()
    {
        dirty = true;
    }

    bool needsRedraw() const
    {
        return dirty;
    }

    const Rect& getBounds() const
    {
        return bounds;
    }

protected:

    Rect bounds;

    bool dirty = true;
};