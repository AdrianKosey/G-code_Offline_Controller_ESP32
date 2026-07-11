#include "home_screen.h"

static constexpr int16_t CONTENT_X = 60;

HomeScreen::HomeScreen()
    : title(Rect{CONTENT_X + 10, 4, 150, 20}, "CONTROL CNC", Theme::Text, 2),
      sdIcon(Rect{CONTENT_X + 205, 6, 16, 16}, Icons::MicroSd, Icons::HEADER_WIDTH, Icons::HEADER_HEIGHT, Theme::TextSecondary),
      wifiIcon(Rect{CONTENT_X + 230, 6, 16, 16}, Icons::Wifi, Icons::HEADER_WIDTH, Icons::HEADER_HEIGHT, Theme::TextSecondary),
      statusBadge(Rect{CONTENT_X + 170, 34, 80, 22}, "RUNNING", Theme::Text, 2),

      jobPanel(Rect{CONTENT_X + 8, 34, 150, 200}, Theme::Panel, 10),
      jobCaption(Rect{CONTENT_X + 15, 40, 130, 16}, "En Curso:", Theme::TextSecondary, 1, Theme::Panel),
      jobFilename(Rect{CONTENT_X + 15, 58, 130, 20}, "Test_router3018.nc", Theme::Text, 2, Theme::Panel),
      jobProgress(Rect{CONTENT_X + 15, 156, 130, 10}, Theme::JobPanel, Theme::Progress),
      jobProgressText(Rect{CONTENT_X + 18, 168, 130, 14}, "1000 / 16504     70%", Theme::TextSecondary, 1, Theme::Panel),

      xyzPanel(Rect{CONTENT_X + 170, 62, 80, 100}, Theme::Panel, 10),
      labelX(Rect{CONTENT_X + 178, 68, 30, 14}, "X", Theme::TextSecondary, 1, Theme::Panel),
      valueX(Rect{CONTENT_X + 178, 80, 64, 16}, "154.000", Theme::Text, 2, Theme::Panel),
      labelY(Rect{CONTENT_X + 178, 100, 30, 14}, "Y", Theme::TextSecondary, 1, Theme::Panel),
      valueY(Rect{CONTENT_X + 178, 112, 64, 16}, "243.670", Theme::Text, 2, Theme::Panel),
      labelZ(Rect{CONTENT_X + 178, 132, 30, 14}, "Z", Theme::TextSecondary, 1, Theme::Panel),
      valueZ(Rect{CONTENT_X + 178, 144, 64, 16}, "-32.450", Theme::Text, 2, Theme::Panel),

      speedLabel(Rect{CONTENT_X + 170, 168, 60, 16}, "Speed", Theme::TextSecondary, 1),
      speedBar(Rect{CONTENT_X + 170, 184, 80, 8}, Theme::JobPanel, Theme::Progress),

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

    playButton.setOnPress([]() { /* TODO: iniciar job */ });
    pauseButton.setOnPress([]() { /* TODO: pausar */ });
    stopButton.setOnPress([]() { /* TODO: detener */ });

    // Test data - replace when the G-code parser delivers real data
    gcodePreview.setProjectBounds(0.0f, 0.0f, 300.0f, 180.0f);
    gcodePreview.addPoint(10, 10);
    gcodePreview.addPoint(290, 10);
    gcodePreview.addPoint(290, 170);
    gcodePreview.addPoint(10, 170);
    gcodePreview.addPoint(10, 10);
    gcodePreview.setCursor(154.0f, 90.0f); 

    // Order = z-order of drawing and touch priority
    widgets = {
        &jobPanel, &xyzPanel,
        &title, &sdIcon, &wifiIcon, &statusBadge,
        &jobCaption, &jobFilename,
        &gcodePreview,
        &jobProgress, &jobProgressText,
        &labelX, &valueX, &labelY, &valueY, &labelZ, &valueZ,
        &speedLabel, &speedBar,
        &playButton, &pauseButton, &stopButton};
}