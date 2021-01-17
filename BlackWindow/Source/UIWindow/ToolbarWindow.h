#pragma once
#include "DefaultImGuiWindow.h"

enum class TIMERSTATE{
    RUNNING,
    STOP,
    PAUSE
};

class ToolbarWindow : public DefaultImGuiWindow
{
public:
    ToolbarWindow(
        const char *_title,
        ImGuiWindowFlags _window_flags) : DefaultImGuiWindow(_title, _window_flags){
            timer = TIMERSTATE::STOP;
        };
    void Update() override;
    TIMERSTATE timer;  
};
