#pragma once

#include <SD.h>

#include "../../core/screen.h"
#include "../../theme.h"
#include "../../widgets/label/label_widget.h"
#include "../../widgets/icon_button/icon_button_widget.h"
#include "../../icons.h"
#include "../../widgets/file_list/file_list_widget.h"
#include "../../widgets/icon/icon_widget.h"

class FilesScreen : public IScreen
{
public:
    FilesScreen();

    void onEnter() override;

private:
    LabelWidget pathLabel;
    IconButtonWidget backButton;
    FileListWidget fileList;

    String currentPath = "/";

    void loadDirectory(const String &path);
};