#pragma once

#include <WebServer.h>
#include <SD.h>
#include "../machine/grbl_controller.h"
#include "../gcode/gcode_job_runner.h"
#include "../machine/framing_runner.h"
#include "../app/app_settings_manager.h"

class WebServerManager
{
public:
    WebServerManager(
        GrblController& grbl,
        GCodeJobRunner& jobRunner,
        FramingRunner& framingRunner,
        AppSettingsManager& appSettings);

    void begin();
    void update();

// The "select file" flow from the web needs the same
// analysis path that the app already uses (bounding box, etc.) - it's resolved via a callback to the app
    using FileSelectCallback = std::function<void(const String&)>;
    void setOnFileSelected(FileSelectCallback callback);

private:
    WebServer server;

    GrblController& grbl;
    GCodeJobRunner& jobRunner;
    FramingRunner& framingRunner;
    AppSettingsManager& appSettings;

    FileSelectCallback onFileSelected;

    // Upload status: In progress
    File uploadFile;

    void handleRoot();
    void handleStatus();
    void handleFiles();
    void handleControl();
    void handleSelect();
    void handleUpload();
    void handleUploadData();
    void handleNotFound();
    void handleDelete();
    void handleRename();
    void handleCreateFolder();
    void handleJog();
    void handleHome();
    void handleSetZero();
    void handleProbeZ();

    String buildFilesJson(const String& path);

    String buildFilesJson();
};