#include "web_server_manager.h"
#include "web_page.h"

WebServerManager::WebServerManager(
    GrblController& grblController,
    GCodeJobRunner& runner,
    FramingRunner& framing,
    AppSettingsManager& settings)
    : server(80), grbl(grblController), jobRunner(runner), framingRunner(framing), appSettings(settings)
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
    json += "\"filename\":\"\",";
    json += "\"line\":" + String(line) + ",";
    json += "\"totalLines\":" + String(total) + ",";
    json += "\"percent\":" + String((int)percent) + ",";
    json += "\"x\":\"" + String(status.x, 3) + "\",";
    json += "\"y\":\"" + String(status.y, 3) + "\",";
    json += "\"z\":\"" + String(status.z, 3) + "\",";
    json += "\"feed\":\"" + String((int)status.feedRate) + "\"";
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
    server.send(200, "application/json", buildFilesJson(path));
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
        // Nota: requiere que el HomeScreen ya haya cargado un archivo con bounds validos -
        // esta version basica no valida eso aqui, se apoya en que framingRunner ya tenga los limites
        // seteados desde la ultima vez que se cargo un archivo por pantalla o por seleccion web.
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
        onFileSelected(path);

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
        uploadFile = SD.open(fullPath, FILE_WRITE);
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        if (uploadFile)
            uploadFile.write(upload.buf, upload.currentSize);
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        if (uploadFile)
            uploadFile.close();
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

    if (path.length() == 0)
    {
        server.send(400, "text/plain", "path requerido");
        return;
    }

    bool ok;
    File f = SD.open(path);
    bool isDir = f && f.isDirectory();
    if (f) f.close();

    if (isDir)
        ok = SD.rmdir(path); // solo funciona si la carpeta esta vacia - SD.h no borra recursivo
    else
        ok = SD.remove(path);

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

    bool ok = SD.rename(oldPath, newPath);
    server.send(ok ? 200 : 500, "text/plain", ok ? "ok" : "error");
}

void WebServerManager::handleCreateFolder()
{
    String path = server.arg("path");

    if (path.length() == 0)
    {
        server.send(400, "text/plain", "path requerido");
        return;
    }

    bool ok = SD.mkdir(path);
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