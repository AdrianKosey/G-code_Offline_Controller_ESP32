#include "files_screen.h"

static constexpr int16_t CONTENT_X = 60;

FilesScreen::FilesScreen()
    : pathLabel(Rect{CONTENT_X + 44, 30, 200, 16}, "/", Theme::TextSecondary, 1, Theme::Background),
      backButton(
          Rect{CONTENT_X + 8, 26, 28, 24},
          Icons::Back, Icons::HEADER_WIDTH, Icons::HEADER_HEIGHT,
          Theme::Background,
          Theme::Text,
          false),
      fileList(Rect{CONTENT_X + 8, 56, 244, 178}, 28)
{
    backButton.setOnPress([this]()
                          {
        if (currentPath == "/")
            return;

        int lastSlash = currentPath.lastIndexOf('/', currentPath.length() - 2);
        currentPath = (lastSlash <= 0) ? "/" : currentPath.substring(0, lastSlash + 1);

        loadDirectory(currentPath); });

    fileList.setOnSelect([this](const FileEntry &entry)
                         {
        if (entry.type == FileEntryType::Folder)
        {
            currentPath += entry.name + "/";
            loadDirectory(currentPath);
        }
        else
        {
            // TODO: Define what happens when you touch a .nc file
        } });

    widgets = {&pathLabel, &backButton, &fileList};
}

void FilesScreen::onEnter()
{
    IScreen::onEnter(); // invalidates all widgets in the vector (behavior inherited from IScreen)
    loadDirectory(currentPath);
}

void FilesScreen::loadDirectory(const String &path)
{
    pathLabel.setText(path);
    File dir = SD.open(path);
    if (!dir || !dir.isDirectory())
    {
        FileEntry errorEntry[1] = {{"No se detecta la tarjeta SD", FileEntryType::File}};
        fileList.setEntries(errorEntry, 1);
        return;
    }
    FileEntry entries[FileListWidget::MAX_ITEMS];
    uint8_t count = 0;

    File file = dir.openNextFile();

    while (file && count < FileListWidget::MAX_ITEMS)
    {
        entries[count].name = file.name();
        entries[count].type = file.isDirectory() ? FileEntryType::Folder : FileEntryType::File;
        count++;

        file = dir.openNextFile();
    }

    fileList.setEntries(entries, count);
}