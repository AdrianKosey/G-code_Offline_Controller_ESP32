#include "web_server_manager.h"
#include "web_page.h"
#include "task_manager_page.h"
#include <SPIFFS.h>
#include <WiFi.h>

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

void WebServerManager::setActiveFile(StorageSource source, const String& path)
{
    activeFileSource = source;
    activeFilePath = path;
}

void WebServerManager::begin()
{
    if (!SPIFFS.begin(false))
        Serial.println("SPIFFS: mount failed (upload the filesystem image)");

    server.serveStatic("/viewer.html", SPIFFS, "/viewer.html", "max-age=86400");
    server.serveStatic("/three-lite.js", SPIFFS, "/three-lite.js", "max-age=86400");

    server.on("/", HTTP_GET, [this]() { handleRoot(); });
    server.on("/system", HTTP_GET, [this]() { handleSystemPage(); });
    server.on("/api/stats", HTTP_GET, [this]() { handleStats(); });
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
    server.on("/api/preview", HTTP_GET, [this]() { handlePreview(); });
    server.on("/api/file", HTTP_GET, [this]() { handleFileDownload(); });
    server.on("/api/active-file", HTTP_GET, [this]() { handleActiveFile(); });

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

void WebServerManager::handleSystemPage()
{
    server.send_P(200, "text/html", TASK_MANAGER_PAGE);
}

void WebServerManager::handleStats()
{
    uint32_t ramTotal = ESP.getHeapSize();
    uint32_t ramFree = ESP.getFreeHeap();
    uint32_t ramMinFree = ESP.getMinFreeHeap();
    uint32_t psramTotal = ESP.getPsramSize();
    uint32_t psramFree = ESP.getFreePsram();

    uint32_t cpuFreq = ESP.getCpuFreqMHz();
    uint32_t uptimeSecs = millis() / 1000;

    float temp = 0.0f;
#if defined(CONFIG_IDF_TARGET_ESP32)
    temp = temperatureRead();
#endif

    size_t spiffsTotal = SPIFFS.totalBytes();
    size_t spiffsUsed = SPIFFS.usedBytes();

    String mac = WiFi.macAddress();
    String ip = WiFi.localIP().toString();
    int32_t rssi = WiFi.RSSI();
    String modeStr = (WiFi.getMode() == WIFI_AP) ? "AP Mode" : "WiFi STA";

    // Recolección de tareas FreeRTOS
    UBaseType_t taskCount = uxTaskGetNumberOfTasks();
    TaskStatus_t* taskStatusArray = (TaskStatus_t*)pvPortMalloc(taskCount * sizeof(TaskStatus_t));

    String tasksJson = "[";
    if (taskStatusArray != NULL)
    {
        taskCount = uxTaskGetSystemState(taskStatusArray, taskCount, NULL);
        for (UBaseType_t i = 0; i < taskCount; i++)
        {
            tasksJson += "{";
            tasksJson += "\"name\":\"" + String(taskStatusArray[i].pcTaskName) + "\",";
            String stateStr;
            switch (taskStatusArray[i].eCurrentState)
            {
                case eRunning:   stateStr = "Running";   break;
                case eReady:     stateStr = "Ready";     break;
                case eBlocked:   stateStr = "Blocked";   break;
                case eSuspended: stateStr = "Suspended"; break;
                default:         stateStr = "Deleted";   break;
            }
            tasksJson += "\"state\":\"" + stateStr + "\",";
            tasksJson += "\"priority\":" + String(taskStatusArray[i].uxCurrentPriority) + ",";
            tasksJson += "\"stack_high_watermark\":" + String(taskStatusArray[i].usStackHighWaterMark) + ",";
            tasksJson += "\"core\":" + String(taskStatusArray[i].xCoreID);
            tasksJson += "}";
            if (i < taskCount - 1) tasksJson += ",";
        }
        vPortFree(taskStatusArray);
    }
    tasksJson += "]";

    String json = "{";
    json += "\"ram\":{\"total\":" + String(ramTotal) + ",\"free\":" + String(ramFree) + ",\"min_free\":" + String(ramMinFree) + "},";
    json += "\"psram\":{\"total\":" + String(psramTotal) + ",\"free\":" + String(psramFree) + "},";
    json += "\"spiffs\":{\"total\":" + String(spiffsTotal) + ",\"used\":" + String(spiffsUsed) + "},";
    json += "\"hardware\":{\"cpu_freq\":" + String(cpuFreq) + ",\"uptime\":" + String(uptimeSecs) + ",\"temp\":" + String(temp) + "},";
    json += "\"network\":{";
    json += "\"mode\":\"" + modeStr + "\",";
    json += "\"ip\":\"" + ip + "\",";
    json += "\"mac\":\"" + mac + "\",";
    json += "\"rssi\":" + String(rssi);
    json += "},";
    json += "\"tasks\":" + tasksJson;
    json += "}";

    server.send(200, "application/json", json);
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
            include = lower.endsWith(".nc") || lower.endsWith(".gcode") || lower.endsWith(".gco") ||
                      lower.endsWith(".ngc") || lower.endsWith(".tap") || lower.endsWith(".cnc") ||
                      lower.endsWith(".gc") || lower.endsWith(".g");
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
                include = lower.endsWith(".nc") || lower.endsWith(".gcode") || lower.endsWith(".gco") ||
                          lower.endsWith(".ngc") || lower.endsWith(".tap") || lower.endsWith(".cnc") ||
                          lower.endsWith(".gc") || lower.endsWith(".g");
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

void WebServerManager::handlePreview()
{
    const GCodePoint* points = homeScreen.getPreviewPoints();
    uint16_t count = homeScreen.getPreviewPointCount();

    String json = "{";
    json += "\"minX\":" + String(homeScreen.getProjectMinX(), 3) + ",";
    json += "\"minY\":" + String(homeScreen.getProjectMinY(), 3) + ",";
    json += "\"maxX\":" + String(homeScreen.getProjectMaxX(), 3) + ",";
    json += "\"maxY\":" + String(homeScreen.getProjectMaxY(), 3) + ",";
    json += "\"points\":[";

    for (uint16_t i = 0; i < count; i++)
    {
        if (i > 0) json += ",";
        json += "[" + String(points[i].x, 3) + "," + String(points[i].y, 3) + "]";
    }

    json += "]}";
    server.send(200, "application/json", json);
}

void WebServerManager::handleFileDownload()
{
    String name = server.arg("name");
    StorageSource source = parseSource();

    // The viewer accepts a self-describing path such as /sd/job.nc or
    // /usb/job.nc, while retaining source=sd|usb for callers that use the
    // regular storage paths.
    if (name.startsWith("/sd/"))
    {
        source = StorageSource::SD;
        name = name.substring(3);
    }
    else if (name.startsWith("/usb/"))
    {
        source = StorageSource::USB;
        name = name.substring(4);
    }

    if (name.length() == 0 || !name.startsWith("/") || name.indexOf("..") >= 0)
    {
        server.send(400, "text/plain", "ruta invalida");
        return;
    }

    IStorageDriver& driver = storage.getDriver(source);
    if (!driver.isAvailable())
    {
        server.send(503, "text/plain", "medio no disponible");
        return;
    }

    IStorageFile* file = driver.openRead(name);
    if (!file || !file->isValid())
    {
        if (file) delete file;
        server.send(404, "text/plain", "archivo no encontrado");
        return;
    }

    // Chunked transfer keeps the ESP32 memory usage bounded. The extra header
    // lets the browser calculate percentage progress even without a HTTP
    // Content-Length header.
    server.sendHeader("X-File-Size", String(file->size()));
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/plain; charset=utf-8", "");

    uint8_t buffer[512];
    while (file->available())
    {
        size_t count = file->read(buffer, sizeof(buffer));
        if (count == 0)
            break;
        server.sendContent((const char*)buffer, count);
        yield();
    }

    file->close();
    delete file;
}

void WebServerManager::handleActiveFile()
{
    if (activeFilePath.length() == 0)
    {
        server.send(404, "application/json", "{\"error\":\"no hay trabajo cargado\"}");
        return;
    }

    String path = activeFilePath;
    path.replace("\\", "\\\\");
    path.replace("\"", "\\\"");
    const char* source = activeFileSource == StorageSource::USB ? "usb" : "sd";
    server.send(200, "application/json", String("{\"source\":\"") + source +
        "\",\"path\":\"" + path + "\"}");
}
