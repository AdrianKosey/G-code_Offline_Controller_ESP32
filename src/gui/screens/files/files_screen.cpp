#include "files_screen.h"

static constexpr int16_t CONTENT_X = 60;

namespace
{
    bool isGCodeFile(const String& filename)
    {
        String lower = filename;
        lower.toLowerCase();

        return lower.endsWith(".nc") ||
               lower.endsWith(".gcode") ||
               lower.endsWith(".ngc") ||
               lower.endsWith(".tap");
    }
}

FilesScreen::FilesScreen(StorageManager& storageManager)
    : storage(storageManager),

      pathLabel(Rect{CONTENT_X + 8, 56, 244, 16}, "/", Theme::TextSecondary, 1, Theme::Background, true),

      backButton(
          Rect{CONTENT_X + 216, 28, 28, 24},
          Icons::Back, Icons::HEADER_WIDTH, Icons::HEADER_HEIGHT,
          Theme::Background,
          Theme::Text,
          false),

      sdTabButton(Rect{CONTENT_X + 8, 30, 60, 24}, "SD"),
      usbTabButton(Rect{CONTENT_X + 72, 30, 60, 24}, "USB"),

      fileList(Rect{CONTENT_X + 8, 76, 244, 158}, 28)
{
    backButton.setOnPress([this]() {
        if (currentPath == "/")
            return;

        int lastSlash = currentPath.lastIndexOf('/', currentPath.length() - 2);
        currentPath = (lastSlash <= 0) ? "/" : currentPath.substring(0, lastSlash + 1);

        loadDirectory(currentPath);
    });

    fileList.setOnSelect([this](const FileEntry& entry) {
        if (entry.type == FileEntryType::Folder)
        {
            currentPath += entry.name + "/";
            loadDirectory(currentPath);
        }
        else
        {
            if (onFileSelected)
                onFileSelected(currentPath + entry.name);
        }
    });

    sdTabButton.setSelected(true);

    sdTabButton.setOnPress([this]() {
        currentSource = StorageSource::SD;
        sdTabButton.setSelected(true);
        usbTabButton.setSelected(false);
        currentPath = "/";
        loadDirectory(currentPath);
    });

    usbTabButton.setOnPress([this]() {
        currentSource = StorageSource::USB;
        sdTabButton.setSelected(false);
        usbTabButton.setSelected(true);
        currentPath = "/";
        loadDirectory(currentPath);
    });

    widgets = { &sdTabButton, &usbTabButton, &backButton, &pathLabel, &fileList };
}

void FilesScreen::onEnter()
{
    IScreen::onEnter();
    loadDirectory(currentPath);
}

void FilesScreen::loadDirectory(const String& path)
{
    pathLabel.setText(path);

    IStorageDriver& driver = storage.getDriver(currentSource);

    if (!driver.isAvailable())
    {
        FileEntry errorEntry[1] = { { "No Detected.", FileEntryType::File } };
        fileList.setEntries(errorEntry, 1);
        return;
    }

    auto entries = driver.listDir(path);

    FileEntry mapped[FileListWidget::MAX_ITEMS];
    uint8_t count = 0;

    for (auto& e : entries)
    {
        bool include = e.isDirectory || isGCodeFile(e.name);

        if (include && count < FileListWidget::MAX_ITEMS)
        {
            mapped[count].name = e.name;
            mapped[count].type = e.isDirectory ? FileEntryType::Folder : FileEntryType::File;
            count++;
        }
    }

    fileList.setEntries(mapped, count);
}

void FilesScreen::setOnFileSelected(FileSelectedCallback callback)
{
    onFileSelected = callback;
}