#include "home_screen.h"

static constexpr int16_t CONTENT_X = 60;

HomeScreen::HomeScreen()
    : statusBadge(Rect{CONTENT_X + 170, 34, 80, 22}, "RUNNING", Theme::Text, 2),

      jobPanel(Rect{CONTENT_X + 8, 34, 150, 200}, Theme::Panel, 10),
      jobCaption(Rect{CONTENT_X + 15, 40, 130, 16}, "En Curso:", Theme::TextSecondary, 1, Theme::Panel, false),
      jobFilename(Rect{CONTENT_X + 15, 54, 130, 20}, "Test_router3018.nc", Theme::Text, 2, Theme::Panel, true),
      jobProgress(Rect{CONTENT_X + 15, 156, 130, 10}, Theme::JobPanel, Theme::Progress),
      jobProgressText(Rect{CONTENT_X + 15, 168, 90, 14}, "1000 / 16504", Theme::TextSecondary, 1, Theme::Panel, false),
      jobProgressPercentage(Rect{CONTENT_X + 115, 168, 40, 14}, "100%", Theme::TextSecondary, 1, Theme::Panel, false),

      xyzPanel(Rect{CONTENT_X + 170, 62, 80, 136}, Theme::Panel, 10),
      labelX(Rect{CONTENT_X + 178, 68, 30, 14}, "X", Theme::TextSecondary, 1, Theme::Panel, false),
      valueX(Rect{CONTENT_X + 178, 80, 64, 16}, "154.000", Theme::Text, 2, Theme::Panel, false),
      labelY(Rect{CONTENT_X + 178, 100, 30, 14}, "Y", Theme::TextSecondary, 1, Theme::Panel, false),
      valueY(Rect{CONTENT_X + 178, 112, 64, 16}, "243.670", Theme::Text, 2, Theme::Panel, false),
      labelZ(Rect{CONTENT_X + 178, 132, 30, 14}, "Z", Theme::TextSecondary, 1, Theme::Panel, false),
      valueZ(Rect{CONTENT_X + 178, 144, 64, 16}, "-32.450", Theme::Text, 2, Theme::Panel, false),
      labelS(Rect{CONTENT_X + 178, 164, 30, 14}, "Feed", Theme::TextSecondary, 1, Theme::Panel, false),
      valueS(Rect{CONTENT_X + 178, 176, 64, 16}, "0", Theme::Text, 2, Theme::Panel, false),

      powerLabel(Rect{CONTENT_X + 170, 200, 60, 16}, "Power", Theme::TextSecondary, 1, Theme::Background, false),
      powerBar(Rect{CONTENT_X + 170, 216, 80, 8}, Theme::JobPanel, Theme::Progress),

      gcodePreview(
          Rect{CONTENT_X + 18, 80, 130, 70},
          Theme::Background,
          Theme::TextSecondary,
          Theme::Text),

      playButton(
          Rect{CONTENT_X + 14, 188, 40, 40},
          Icons::Play, Icons::PLAYBACK_WIDTH, Icons::PLAYBACK_HEIGHT,
          Theme::ButtonPlayBackground,
          Theme::Text),

      pauseButton(
          Rect{CONTENT_X + 62, 188, 40, 40},
          Icons::Pause, Icons::PLAYBACK_WIDTH, Icons::PLAYBACK_HEIGHT,
          Theme::ButtonPauseBackground,
          Theme::Text),

      stopButton(
          Rect{CONTENT_X + 110, 188, 40, 40},
          Icons::Stop, Icons::PLAYBACK_WIDTH, Icons::PLAYBACK_HEIGHT,
          Theme::ButtonStopBackground,
          Theme::Text)
{
    statusBadge.setBadge(Theme::Progress, 11);

    playButton.setOnPress([this]()
                          { if (onPlay) onPlay(); });
    pauseButton.setOnPress([this]()
                           { if (onPause) onPause(); });
    stopButton.setOnPress([this]()
                          { if (onStop) onStop(); });

    // Order = z-order of drawing and touch priority
    widgets = {
        &jobPanel, &xyzPanel, &statusBadge,
        &jobCaption, &jobFilename, &jobProgressPercentage,
        &gcodePreview,
        &jobProgress, &jobProgressText,
        &labelX, &valueX, &labelY, &valueY, &labelZ, &valueZ, &labelS, &valueS,
        &powerLabel, &powerBar,
        &playButton, &pauseButton, &stopButton};
}

void HomeScreen::loadJob(const String &path)
{
    GCodeFileInfo info = GCodeFileAnalyzer::analyze(path);

    if (!info.valid)
    {
        jobFilename.setText("Archivo invalido");
        gcodePreview.clearPath();
        return;
    }

    int lastSlash = path.lastIndexOf('/');
    String filename = (lastSlash >= 0) ? path.substring(lastSlash + 1) : path;
    jobFilename.setText(filename);

    totalLines = info.totalLines;
    currentLine = 0;

    gcodePreview.clearPath();
    gcodePreview.setProjectBounds(info.minX, info.minY, info.maxX, info.maxY);


    uint32_t stride = max((uint32_t)1, info.totalLines / GCodePreviewWidget::MAX_POINTS);

    File file = SD.open(path);
    if (!file)
        return;

    GCodeParser parser;
    uint32_t lineIndex = 0;

    while (file.available())
    {
        String line = file.readStringUntil('\n');
        GCodeCommand command = parser.parseLine(line);

        if (command.hasTarget && (lineIndex % stride == 0))
            gcodePreview.addPoint(command.target.x, command.target.y);

        lineIndex++;
    }

    file.close();

    gcodePreview.setProgress(0);
    jobProgress.setValue(0);
    jobProgressText.setText("0 / " + String(totalLines));
    jobProgressPercentage.setText("0%");
}

void HomeScreen::setOnPlay(ActionCallback callback) { onPlay = callback; }
void HomeScreen::setOnPause(ActionCallback callback) { onPause = callback; }
void HomeScreen::setOnStop(ActionCallback callback) { onStop = callback; }

uint32_t HomeScreen::getTotalLines() const { return totalLines; }

void HomeScreen::updateMachineState(JobState jobState, const GrblStatus &status, uint32_t currentLine, uint32_t totalLinesParam)
{
    valueX.setText(String(status.x, 3));
    valueY.setText(String(status.y, 3));
    valueZ.setText(String(status.z, 3));

    valueS.setText(String((int)status.feedRate));

    float powerPercent = constrain(status.spindleSpeed / MAX_SPINDLE_SPEED * 100.0f, 0.0f, 100.0f);
    powerBar.setValue(powerPercent);

    gcodePreview.setCursor(status.x, status.y);

    String stateText;
    uint16_t stateColor;

    switch (jobState)
    {
    case JobState::Running:
        stateText = "RUNNING";
        stateColor = Theme::Success;
        break;
    case JobState::Paused:
        stateText = "PAUSADO";
        stateColor = Theme::Warning;
        break;
    case JobState::Completed:
        stateText = "TERMINADO";
        stateColor = Theme::Progress;
        break;
    case JobState::Error:
        stateText = "ERROR";
        stateColor = Theme::Error;
        break;
    default:
        stateText = "LISTO";
        stateColor = Theme::TextSecondary;
        break;
    }

    statusBadge.setText(stateText);
    statusBadge.setBadge(stateColor, 11);

    if (totalLinesParam > 0)
    {
        float percent = (float)currentLine * 100.0f / (float)totalLinesParam;

        jobProgress.setValue(percent);
        jobProgressText.setText(String(currentLine) + " / " + String(totalLinesParam));
        jobProgressPercentage.setText(String((int)percent) + "%");

        gcodePreview.setProgress(percent);
    }
}