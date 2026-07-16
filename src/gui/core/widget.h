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

    void setBounds(const Rect& newBounds)
    {
        if (bounds.x == newBounds.x && bounds.y == newBounds.y &&
            bounds.width == newBounds.width && bounds.height == newBounds.height)
            return;

        bounds = newBounds;
        invalidate();
    }

    const Rect& getBounds() const
    {
        return bounds;
    }

    void setPosition(int16_t x, int16_t y)
    {
        if (bounds.x == x && bounds.y == y)
            return;

        bounds.x = x;
        bounds.y = y;
        invalidate();
    }

protected:

    Rect bounds;

    bool dirty = true;
};