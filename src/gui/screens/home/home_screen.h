#pragma once

#include "../../core/screen.h"
#include "../../theme.h"
#include "../../icons.h"

#include "../../widgets/panel/panel_widget.h"
#include "../../widgets/label/label_widget.h"
#include "../../widgets/progress_bar/progress_bar_widget.h"
#include "../../widgets/icon_button/icon_button_widget.h"
#include "../../widgets/icon/icon_widget.h"
#include "../../widgets/gcode_preview/gcode_preview_widget.h"

class HomeScreen : public IScreen
{
public:
    HomeScreen();
    
private:
    // Header
    LabelWidget statusBadge;

    // Panel "In Progress Job"
    PanelWidget jobPanel;
    LabelWidget jobCaption;
    LabelWidget jobFilename;
    ProgressBarWidget jobProgress;
    LabelWidget jobProgressText;

    // Panel XYZ
    PanelWidget xyzPanel;
    LabelWidget labelX, valueX;
    LabelWidget labelY, valueY;
    LabelWidget labelZ, valueZ;

    // Speed
    LabelWidget speedLabel;
    ProgressBarWidget speedBar;

    // Controles
    IconButtonWidget playButton;
    IconButtonWidget pauseButton;
    IconButtonWidget stopButton;

    GCodePreviewWidget gcodePreview;
};