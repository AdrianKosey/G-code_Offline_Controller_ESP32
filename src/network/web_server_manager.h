#pragma once

#include <WebServer.h>
#include <SD.h>
#include "../machine/grbl_controller.h"
#include "../gcode/gcode_job_runner.h"
#include "../machine/framing_runner.h"
#include "../app/app_settings_manager.h"
#include "../gui/screens/home/home_screen.h"
#include "../storage/storage_manager.h"


class WebServerManager
{
public:
    WebServerManager(
        GrblController& grbl,
        GCodeJobRunner& jobRunner,
        FramingRunner& framingRunner,
        AppSettingsManager& appSettings,
        StorageManager& storageManager,
        HomeScreen& homeScreen);

    void begin();
    void update();

// The "select file" flow from the web needs the same
// analysis path that the app already uses (bounding box, etc.) - it's resolved via a callback to the app
    using FileSelectCallback = std::function<void(StorageSource, const String&)>;
    void setOnFileSelected(FileSelectCallback callback);

    // Kept by the application whenever a job is opened, including from the
    // physical display.  The browser uses it to open its 3D preview directly.
    void setActiveFile(StorageSource source, const String& path);

private:
    WebServer server;

    GrblController& grbl;
    GCodeJobRunner& jobRunner;
    FramingRunner& framingRunner;
    AppSettingsManager& appSettings;
    StorageManager& storage;
    HomeScreen& homeScreen;

    FileSelectCallback onFileSelected;
    StorageSource activeFileSource = StorageSource::SD;
    String activeFilePath;

    IStorageFile* uploadStorageFile = nullptr;

    void handleRoot();
    void handleSystemPage();
    void handleStats();
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
    void handleConsoleGet();
    void handleConsoleSend();
    void handlePreview();
    void handleFileDownload();
    void handleActiveFile();
    

    String buildFilesJson(const String& path);

    String buildFilesJson();

    StorageSource parseSource();
};
