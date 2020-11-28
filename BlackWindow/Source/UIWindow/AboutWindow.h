#pragma once
#include "DefaultImGuiWindow.h"
class AboutWindow : public DefaultImGuiWindow
{
public:
    AboutWindow(
        const char *_title,
        ImGuiWindowFlags _window_flags) : DefaultImGuiWindow(_title, _window_flags){};
    void Update() override;
};
