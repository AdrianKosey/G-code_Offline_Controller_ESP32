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
#include "../../../machine/grbl_controller.h"

class HomeScreen : public IScreen
{
public:
    HomeScreen(GrblController& grblController);
    void loadJob(const String& path, bool previewEnabled, bool framingEnabled);
    using ActionCallback = std::function<void()>;

    void setOnPlayPause(ActionCallback callback);
    void setOnFraming(ActionCallback callback);
    void setOnStop(ActionCallback callback);

    float getProjectMinX() const;
    float getProjectMinY() const;
    float getProjectMaxX() const;
    float getProjectMaxY() const;

    uint32_t getTotalLines() const;
    bool hasValidProjectBounds() const;

    void updateMachineState(JobState jobState, const GrblStatus& status, uint32_t currentLine, uint32_t totalLines);
private:
    GrblController& grbl;
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
    IconButtonWidget playPauseButton;
    IconButtonWidget stopButton;
    IconButtonWidget framingButton;

    GCodePreviewWidget gcodePreview;

    uint32_t totalLines = 0;
    uint32_t currentLine = 0;
    bool projectBoundsValid = false;

    ActionCallback onPlayPause; 
    ActionCallback onStop;
    ActionCallback onFraming;

    float projectMinX = 0, projectMinY = 0, projectMaxX = 0, projectMaxY = 0;
};