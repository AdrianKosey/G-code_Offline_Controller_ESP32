#pragma once
#include <SD.h>
#include <functional>
#include "../../../../include/config.h"
#include "../../../machine/grbl_controller.h"
#include "../../../gcode/gcode_job_runner.h"
#include "../../../gcode/gcode_parser.h"
#include "../../../gcode/gcode_file_analyzer.h"
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
    void loadJob(const String& path);
    using ActionCallback = std::function<void()>;

    void setOnPlay(ActionCallback callback);
    void setOnPause(ActionCallback callback);
    void setOnStop(ActionCallback callback);

    uint32_t getTotalLines() const;

    void updateMachineState(JobState jobState, const GrblStatus& status, uint32_t currentLine, uint32_t totalLines);
private:
    // Header
    LabelWidget statusBadge;

    // Panel "In Progress Job"
    PanelWidget jobPanel;
    LabelWidget jobCaption;
    LabelWidget jobFilename;
    ProgressBarWidget jobProgress;
    LabelWidget jobProgressText;
    LabelWidget jobProgressPercentage;

    // Panel XYZ
    PanelWidget xyzPanel;
    LabelWidget labelX, valueX;
    LabelWidget labelY, valueY;
    LabelWidget labelZ, valueZ;
    LabelWidget labelS, valueS;

    // Power
    LabelWidget powerLabel;
    ProgressBarWidget powerBar;

    // Controles
    IconButtonWidget playButton;
    IconButtonWidget pauseButton;
    IconButtonWidget stopButton;

    GCodePreviewWidget gcodePreview;

    uint32_t totalLines = 0;
    uint32_t currentLine = 0;

    ActionCallback onPlay, onPause, onStop;
};