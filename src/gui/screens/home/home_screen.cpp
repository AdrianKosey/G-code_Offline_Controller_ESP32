#include "home_screen.h"
#include "../../../i18n/translations.h"

static constexpr int16_t CONTENT_X = 60;

HomeScreen::HomeScreen(GrblController& grblController)
    : grbl(grblController),
      statusBadge(Rect{CONTENT_X + 170, 34, 80, 22}, tr(StringId::Home_Ready), Theme::Text, 2),
      jobPanel(Rect{CONTENT_X + 8, 34, 150, 200}, Theme::Panel, 10),
      jobCaption(Rect{CONTENT_X + 15, 40, 130, 16}, tr(StringId::Home_InProgress), Theme::TextSecondary, 1, Theme::Panel, false),
      jobFilename(Rect{CONTENT_X + 15, 54, 130, 20}, tr(StringId::Home_NoFile), Theme::Text, 2, Theme::Panel, true),
      jobProgress(Rect{CONTENT_X + 15, 156, 130, 10}, Theme::JobPanel, Theme::Progress),
      jobProgressText(Rect{CONTENT_X + 15, 168, 90, 14}, "0 / 0", Theme::TextSecondary, 1, Theme::Panel, false),
      jobProgressPercentage(Rect{CONTENT_X + 115, 168, 40, 14}, "0%", Theme::TextSecondary, 1, Theme::Panel, false),

      xyzPanel(Rect{CONTENT_X + 170, 62, 80, 136}, Theme::Panel, 10),
      labelX(Rect{CONTENT_X + 178, 68, 30, 14}, tr(StringId::Coords_X), Theme::TextSecondary, 1, Theme::Panel, false),
      valueX(Rect{CONTENT_X + 178, 80, 64, 16}, "0.000", Theme::Text, 2, Theme::Panel, false),
      labelY(Rect{CONTENT_X + 178, 100, 30, 14}, tr(StringId::Coords_Y), Theme::TextSecondary, 1, Theme::Panel, false),
      valueY(Rect{CONTENT_X + 178, 112, 64, 16}, "0.000", Theme::Text, 2, Theme::Panel, false),
      labelZ(Rect{CONTENT_X + 178, 132, 30, 14}, tr(StringId::Coords_Z), Theme::TextSecondary, 1, Theme::Panel, false),
      valueZ(Rect{CONTENT_X + 178, 144, 64, 16}, "0.000", Theme::Text, 2, Theme::Panel, false),
      labelS(Rect{CONTENT_X + 178, 164, 30, 14}, tr(StringId::Home_Speed), Theme::TextSecondary, 1, Theme::Panel, false),
      valueS(Rect{CONTENT_X + 178, 176, 64, 16}, "0", Theme::Text, 2, Theme::Panel, false),

      powerLabel(Rect{CONTENT_X + 170, 200, 60, 16}, tr(StringId::Home_Power), Theme::TextSecondary, 1, Theme::Background, false),
      powerBar(Rect{CONTENT_X + 170, 216, 80, 8}, Theme::JobPanel, Theme::Progress),

      gcodePreview(
          Rect{CONTENT_X + 18, 80, 130, 70},
          Theme::Background,
          Theme::TextSecondary,
          Theme::Text),

      playPauseButton(
          Rect{CONTENT_X + 14, 188, 40, 40},
          Icons::Play, Icons::PLAYBACK_WIDTH, Icons::PLAYBACK_HEIGHT,
          Theme::ButtonPlayBackground,
          Theme::Text),

      stopButton(
          Rect{CONTENT_X + 62, 188, 40, 40},
          Icons::Stop, Icons::PLAYBACK_WIDTH, Icons::PLAYBACK_HEIGHT,
          Theme::ButtonStopBackground,
          Theme::Text),

        framingButton(
              Rect{CONTENT_X + 110, 188, 40, 40},
              Icons::Framing, Icons::PLAYBACK_WIDTH, Icons::PLAYBACK_HEIGHT,
              Theme::ButtonPlayBackground,
              Theme::Text)
{
    statusBadge.setBadge(Theme::Progress, 11);

    playPauseButton.setOnPress([this]()
                               { if (onPlayPause) onPlayPause(); });
    stopButton.setOnPress([this]()
                          { if (onStop) onStop(); });
    framingButton.setOnPress([this]()
                             { if (onFraming) onFraming(); });

    // Order = z-order of drawing and touch priority
    widgets = {
        &jobPanel, &xyzPanel, &statusBadge,
        &jobCaption, &jobFilename, &jobProgressPercentage,
        &gcodePreview,
        &jobProgress, &jobProgressText,
        &labelX, &valueX, &labelY, &valueY, &labelZ, &valueZ, &labelS, &valueS,
        &powerLabel, &powerBar,
        &playPauseButton, &stopButton, &framingButton};
}

void HomeScreen::loadJob(const String &path)
{
    GCodeFileInfo info = GCodeFileAnalyzer::analyze(path);

    if (!info.valid)
    {
        jobFilename.setText(tr(StringId::Files_Invalid));
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

    projectMinX = info.minX;
    projectMinY = info.minY;
    projectMaxX = info.maxX;
    projectMaxY = info.maxY;

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

void HomeScreen::setOnPlayPause(ActionCallback callback) { onPlayPause = callback; }
void HomeScreen::setOnFraming(ActionCallback callback) { onFraming = callback; }
void HomeScreen::setOnStop(ActionCallback callback) { onStop = callback; }

float HomeScreen::getProjectMinX() const { return projectMinX; }
float HomeScreen::getProjectMinY() const { return projectMinY; }
float HomeScreen::getProjectMaxX() const { return projectMaxX; }
float HomeScreen::getProjectMaxY() const { return projectMaxY; }

uint32_t HomeScreen::getTotalLines() const { return totalLines; }

void HomeScreen::updateMachineState(JobState jobState, const GrblStatus &status, uint32_t currentLine, uint32_t totalLinesParam)
{
    valueX.setText(String(status.x, 3));
    valueY.setText(String(status.y, 3));
    valueZ.setText(String(status.z, 3));

    valueS.setText(String((int)status.feedRate));

    float powerPercent = constrain(status.spindleSpeed / grbl.getMaxSpindleSpeed() * 100.0f, 0.0f, 100.0f);
    powerBar.setValue(powerPercent);

    gcodePreview.setCursor(status.x, status.y);

    String stateText;
    uint16_t stateColor;

    switch (jobState)
    {
    case JobState::Running:
        stateText = tr(StringId::Home_Running);
        stateColor = Theme::Success;
        playPauseButton.setIcon(Icons::Pause);
        break;
    case JobState::Paused:
        stateText = tr(StringId::Home_Paused);
        stateColor = Theme::Warning;
        playPauseButton.setIcon(Icons::Play);
        break;
    case JobState::Completed:
        stateText = tr(StringId::Home_Completed);
        stateColor = Theme::Progress;
        playPauseButton.setIcon(Icons::Play);
        break;
    case JobState::Error:
        stateText = tr(StringId::Home_Error);
        stateColor = Theme::Error;
        playPauseButton.setIcon(Icons::Play);
        break;
    default:
        stateText = tr(StringId::Home_Ready);
        stateColor = Theme::TextSecondary;
        playPauseButton.setIcon(Icons::Play);
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