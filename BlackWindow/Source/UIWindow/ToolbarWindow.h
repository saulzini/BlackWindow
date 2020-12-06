#pragma once
#include "DefaultImGuiWindow.h"
class ToolbarWindow : public DefaultImGuiWindow
{
public:
    ToolbarWindow(
        const char *_title,
        ImGuiWindowFlags _window_flags) : DefaultImGuiWindow(_title, _window_flags){};
    void Update() override;
};
