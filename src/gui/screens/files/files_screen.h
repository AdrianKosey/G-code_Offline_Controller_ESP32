#pragma once

#include <functional>

#include "../../core/screen.h"
#include "../../theme.h"
#include "../../widgets/label/label_widget.h"
#include "../../widgets/icon_button/icon_button_widget.h"
#include "../../widgets/button/button_widget.h"
#include "../../widgets/file_list/file_list_widget.h"
#include "../../icons.h"
#include "../../../storage/storage_manager.h"

class FilesScreen : public IScreen
{
public:
    using FileSelectedCallback = std::function<void(const String&)>;

    FilesScreen(StorageManager& storageManager);

    void onEnter() override;

    void setOnFileSelected(FileSelectedCallback callback);
    StorageSource getCurrentSource() const { return currentSource; }

private:
    StorageManager& storage;
    StorageSource currentSource = StorageSource::SD;

    LabelWidget pathLabel;
    IconButtonWidget backButton;
    ButtonWidget sdTabButton;
    ButtonWidget usbTabButton;
    FileListWidget fileList;

    String currentPath = "/";

    FileSelectedCallback onFileSelected;

    void loadDirectory(const String& path);
    
};