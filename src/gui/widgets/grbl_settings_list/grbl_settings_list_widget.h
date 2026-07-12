#pragma once

#include <Arduino.h>
#include <functional>
#include "../../core/widget.h"
#include "../../theme.h"
#include "../../../machine/grbl_controller.h"
#include "../../../machine/grbl_setting_meta.h"

class GrblSettingsListWidget : public Widget
{
public:
    using SelectCallback = std::function<void(const GrblSettingDef&, float currentValue)>;

    GrblSettingsListWidget(const Rect& bounds, GrblController& grbl, int16_t rowHeight = 40);

    void draw(DisplayManager& display) override;
    bool handleTouch(const TouchEvent& event) override;

    void refresh(); // recalculates total height, call if any external value changes

    void setOnSelect(SelectCallback callback);

private:
    GrblController& grbl;

    int16_t rowHeight;
    int16_t scrollOffset = 0;
    int16_t maxScrollOffset = 0;

    bool dragging = false;
    int16_t lastTouchY = 0;
    int16_t totalDragDistance = 0;

    SelectCallback onSelect;

    static constexpr int16_t TAP_THRESHOLD = 8;

    void recalculateMaxScroll();
    void clampScroll();

    String formatValue(const GrblSettingDef& def, float value) const;
};