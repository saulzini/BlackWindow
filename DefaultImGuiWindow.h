#pragma once
#include "ImGui/imgui.h"

class DefaultImGuiWindow
{
public:
    DefaultImGuiWindow(const char* _title, ImGuiWindowFlags _windowFlags) : title(_title) , window_flags(_windowFlags) {
        show = true;
    }
    ~DefaultImGuiWindow();
    virtual void Update() {};

    const bool getShow() {
        return show;
    }
    ImGuiWindowFlags window_flags;

    void toggleWindow(){
        show = !show;
    }

    void setShowWindow(const bool showWindow){
        show = showWindow;
    }

protected:
    bool begin();
    void end();
    bool show;
    const char* title;
};

