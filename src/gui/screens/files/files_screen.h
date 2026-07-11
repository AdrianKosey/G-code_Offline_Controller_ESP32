#pragma once

#include <SD.h>
#include <functional>

#include "../../core/screen.h"
#include "../../theme.h"
#include "../../widgets/label/label_widget.h"
#include "../../widgets/icon_button/icon_button_widget.h"
#include "../../widgets/file_list/file_list_widget.h"
#include "../../icons.h"

class FilesScreen : public IScreen
{
public:
    using FileSelectedCallback = std::function<void(const String&)>;

    FilesScreen();

    void onEnter() override;

    void setOnFileSelected(FileSelectedCallback callback);

private:
    LabelWidget pathLabel;
    IconButtonWidget backButton;
    FileListWidget fileList;

    String currentPath = "/";

    FileSelectedCallback onFileSelected;

    void loadDirectory(const String& path);
};