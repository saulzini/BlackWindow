#pragma once
#include "ImGui/imgui.h"

class DefaultImGuiWindow
{
public:
    DefaultImGuiWindow(const char* _title, ImGuiWindowFlags _window_flags) : title(_title) , window_flags(_window_flags) {
        show = true;
    }
    ~DefaultImGuiWindow();
    virtual void Update() {};

    const bool getShow() {
        return show;
    }
    ImGuiWindowFlags window_flags;

protected:
    bool begin();
    void end();
    bool show;
    const char* title;
};

