#include "web_server_manager.h"
#include "web_page.h"

WebServerManager::WebServerManager(
    GrblController& grblController,
    GCodeJobRunner& runner,
    FramingRunner& framing,
    AppSettingsManager& settings,
    StorageManager& storageManager,
    HomeScreen& home)
    : server(80), grbl(grblController), jobRunner(runner), framingRunner(framing),
      appSettings(settings), storage(storageManager), homeScreen(home)
{}

void WebServerManager::setOnFileSelected(FileSelectCallback callback)
{
    onFileSelected = callback;
}

void WebServerManager::begin()
{
    server.on("/", HTTP_GET, [this]() { handleRoot(); });
    server.on("/api/status", HTTP_GET, [this]() { handleStatus(); });
    server.on("/api/files", HTTP_GET, [this]() { handleFiles(); });
    server.on("/api/control", HTTP_POST, [this]() { handleControl(); });
    server.on("/api/select", HTTP_POST, [this]() { handleSelect(); });
    server.on("/api/delete", HTTP_POST, [this]() { handleDelete(); });
    server.on("/api/rename", HTTP_POST, [this]() { handleRename(); });
    server.on("/api/mkdir", HTTP_POST, [this]() { handleCreateFolder(); });
    server.on("/api/jog", HTTP_POST, [this]() { handleJog(); });
    server.on("/api/home", HTTP_POST, [this]() { handleHome(); });
    server.on("/api/setzero", HTTP_POST, [this]() { handleSetZero(); });
    server.on("/api/probez", HTTP_POST, [this]() { handleProbeZ(); });
    server.on("/api/console", HTTP_GET, [this]() { handleConsoleGet(); });
    server.on("/api/console/send", HTTP_POST, [this]() { handleConsoleSend(); });

    server.on("/api/upload", HTTP_POST,
        [this]() { handleUpload(); },      // is called when the upload is complete
        [this]() { handleUploadData(); }); // is called repeatedly as bytes arrive

    server.onNotFound([this]() { handleNotFound(); });

    server.begin();
}

void WebServerManager::update()
{
    server.handleClient();
}

void WebServerManager::handleRoot()
{
    server.send_P(200, "text/html", WEB_PAGE_HTML);
}

void WebServerManager::handleStatus()
{
    const GrblStatus& status = grbl.getStatus();
    JobState state = jobRunner.getState();

    uint32_t line = jobRunner.getCurrentLine();
    uint32_t total = jobRunner.getTotalLines();
    float percent = (total > 0) ? ((float)line * 100.0f / (float)total) : 0.0f;

    const char* stateStr = "LISTO";
    switch (state)
    {
        case JobState::Running:   stateStr = "RUNNING"; break;
        case JobState::Paused:    stateStr = "PAUSADO"; break;
        case JobState::Completed: stateStr = "TERMINADO"; break;
        case JobState::Error:     stateStr = "ERROR"; break;
        default: break;
    }

    String json = "{";
    json += "\"state\":\"" + String(stateStr) + "\",";
    json += "\"filename\":\"" + homeScreen.getFilename() + "\",";
    json += "\"line\":" + String(line) + ",";
    json += "\"totalLines\":" + String(total) + ",";
    json += "\"percent\":" + String((int)percent) + ",";
    json += "\"x\":\"" + String(status.x, 3) + "\",";
    json += "\"y\":\"" + String(status.y, 3) + "\",";
    json += "\"z\":\"" + String(status.z, 3) + "\",";
    json += "\"feed\":\"" + String((int)status.feedRate) + "\",";
    json += "\"sdOk\":" + String(storage.isSdAvailable() ? "true" : "false") + ",";
    json += "\"usbOk\":" + String(storage.isUsbAvailable() ? "true" : "false") + ",";
    json += "\"machineOk\":" + String(grbl.getConnectionState() == GrblConnectionState::Connected ? "true" : "false");
    json += "}";

    server.send(200, "application/json", json);
}

String WebServerManager::buildFilesJson(const String& path)
{
    String json = "{\"path\":\"" + path + "\",\"entries\":[";

    File dir = SD.open(path);

    if (!dir || !dir.isDirectory())
    {
        json += "]}";
        return json;
    }

    File file = dir.openNextFile();
    bool first = true;

    while (file)
    {
        String name = file.name();
        bool isDir = file.isDirectory();

        bool include = isDir;

        if (!isDir)
        {
            String lower = name;
            lower.toLowerCase();
            include = lower.endsWith(".nc") || lower.endsWith(".gcode") || lower.endsWith(".ngc") || lower.endsWith(".tap");
        }

        if (include)
        {
            if (!first) json += ",";
            json += "{\"name\":\"" + name + "\",\"isDir\":" + (isDir ? "true" : "false") + "}";
            first = false;
        }

        file = dir.openNextFile();
    }

    dir.close();
    json += "]}";
    return json;
}

void WebServerManager::handleFiles()
{
    String path = server.hasArg("path") ? server.arg("path") : "/";
    IStorageDriver& driver = storage.getDriver(parseSource());

    String json = "{\"path\":\"" + path + "\",\"entries\":[";

    if (driver.isAvailable())
    {
        auto entries = driver.listDir(path);
        bool first = true;

        for (auto& e : entries)
        {
            bool include = e.isDirectory;

            if (!e.isDirectory)
            {
                String lower = e.name;
                lower.toLowerCase();
                include = lower.endsWith(".nc") || lower.endsWith(".gcode") || lower.endsWith(".ngc") || lower.endsWith(".tap");
            }

            if (include)
            {
                if (!first) json += ",";
                json += "{\"name\":\"" + e.name + "\",\"isDir\":" + (e.isDirectory ? "true" : "false") + "}";
                first = false;
            }
        }
    }

    json += "]}";
    server.send(200, "application/json", json);
}

void WebServerManager::handleControl()
{
    String action = server.arg("action");

    if (action == "playpause")
    {
        JobState state = jobRunner.getState();

        if (state == JobState::Running)
            jobRunner.pause();
        else if (state == JobState::Paused)
            jobRunner.resume();
        else
            jobRunner.start();
    }
    else if (action == "stop")
    {
        jobRunner.stop();
    }
    else if (action == "framing")
    {
        // Note: Requires that the HomeScreen has already loaded a file with valid bounds -
        // This basic version doesn't validate that here; it relies on framingRunner already having the bounds set
        // since the last time a file was loaded via screen or web selection.
    }

    server.send(200, "text/plain", "ok");
}

void WebServerManager::handleSelect()
{
    String path = server.arg("path");

    if (path.length() == 0)
    {
        server.send(400, "text/plain", "path requerido");
        return;
    }

    if (onFileSelected)
        onFileSelected(parseSource(), path);

    server.send(200, "text/plain", "ok");
}

void WebServerManager::handleUploadData()
{
    HTTPUpload& upload = server.upload();

    if (upload.status == UPLOAD_FILE_START)
    {
        String targetDir = server.hasArg("path") ? server.arg("path") : "/";
        if (!targetDir.endsWith("/")) targetDir += "/";

        String fullPath = targetDir + upload.filename;

        IStorageDriver& driver = storage.getDriver(parseSource());
        uploadStorageFile = driver.openWrite(fullPath); 
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        if (uploadStorageFile)
            uploadStorageFile->write(upload.buf, upload.currentSize);
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        if (uploadStorageFile)
        {
            uploadStorageFile->close();
            delete uploadStorageFile;
            uploadStorageFile = nullptr;
        }
    }
}

void WebServerManager::handleUpload()
{
    server.send(200, "text/plain", "ok");
}

void WebServerManager::handleNotFound()
{
    server.send(404, "text/plain", "No encontrado");
}

void WebServerManager::handleDelete()
{
    String path = server.arg("path");
    if (path.length() == 0) { server.send(400, "text/plain", "path requerido"); return; }

    IStorageDriver& driver = storage.getDriver(parseSource());

    // I need to know if it's a folder or a file - listDir of the parent, or try removing it first.
    // I simplify: we try remove(); if it fails, we try rmdir()
    bool ok = driver.remove(path);
    if (!ok) ok = driver.rmdir(path);

    server.send(ok ? 200 : 500, "text/plain", ok ? "ok" : "error");
}

void WebServerManager::handleRename()
{
    String oldPath = server.arg("path");
    String newName = server.arg("newName");

    if (oldPath.length() == 0 || newName.length() == 0)
    {
        server.send(400, "text/plain", "parametros requeridos");
        return;
    }

    int lastSlash = oldPath.lastIndexOf('/');
    String parentDir = (lastSlash > 0) ? oldPath.substring(0, lastSlash) : "";
    String newPath = parentDir + "/" + newName;

    IStorageDriver& driver = storage.getDriver(parseSource());
    bool ok = driver.rename(oldPath, newPath);

    server.send(ok ? 200 : 500, "text/plain", ok ? "ok" : "error");
}

void WebServerManager::handleCreateFolder()
{
    String path = server.arg("path");
    if (path.length() == 0) { server.send(400, "text/plain", "path requerido"); return; }

    IStorageDriver& driver = storage.getDriver(parseSource());
    bool ok = driver.mkdir(path);

    server.send(ok ? 200 : 500, "text/plain", ok ? "ok" : "error");
}

void WebServerManager::handleJog()
{
    String axisStr = server.arg("axis");
    float distance = server.arg("distance").toFloat();

    if (axisStr.length() != 1)
    {
        server.send(400, "text/plain", "axis invalido");
        return;
    }

    grbl.jog(axisStr[0], distance, appSettings.getJogFeedRate());
    server.send(200, "text/plain", "ok");
}

void WebServerManager::handleHome()
{
    grbl.home();
    server.send(200, "text/plain", "ok");
}



void WebServerManager::handleSetZero()
{
    String axisStr = server.arg("axis");

    if (axisStr.length() != 1)
    {
        server.send(400, "text/plain", "axis invalido");
        return;
    }

    grbl.setWorkZeroAxis(axisStr[0]);
    server.send(200, "text/plain", "ok");
}

void WebServerManager::handleProbeZ()
{
    grbl.probeZ(15.0f, 100.0f);
    server.send(200, "text/plain", "ok");
}

void WebServerManager::handleConsoleGet()
{
    const auto& history = grbl.getConsoleHistory();

    String json = "{\"version\":" + String(grbl.getConsoleVersion()) + ",\"entries\":[";
    bool first = true;

    for (auto& entry : history)
    {
        if (!first) json += ",";

        String escaped = entry.text;
        escaped.replace("\\", "\\\\");
        escaped.replace("\"", "\\\"");

        json += "{\"out\":" + String(entry.isOutgoing ? "true" : "false") + ",\"text\":\"" + escaped + "\"}";
        first = false;
    }

    json += "]}";
    server.send(200, "application/json", json);
}

void WebServerManager::handleConsoleSend()
{
    String command = server.arg("command");

    if (command.length() == 0)
    {
        server.send(400, "text/plain", "command requerido");
        return;
    }

    // Basic security: avoid sending commands while an automated job is running -
    // Sending manual G-code in the middle of a stream can desynchronize the Grbl buffer
    if (jobRunner.getState() == JobState::Running)
    {
        server.send(409, "text/plain", "No se puede enviar mientras hay un trabajo corriendo");
        return;
    }

    grbl.sendRaw(command);
    server.send(200, "text/plain", "ok");
}

StorageSource WebServerManager::parseSource()
{
    String s = server.hasArg("source") ? server.arg("source") : "sd";
    return (s == "usb") ? StorageSource::USB : StorageSource::SD;
}