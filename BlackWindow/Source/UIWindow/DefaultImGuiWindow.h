#pragma once
#include "imgui.h"
class DefaultImGuiWindow
{

protected:
    bool begin();
    void end();
    bool show;
    const char* title;
public:
    DefaultImGuiWindow(const char* _title, ImGuiWindowFlags _windowFlags) : title(_title) , window_flags(_windowFlags) {
        show = true;
    }
    ~DefaultImGuiWindow();

    ImGuiWindowFlags window_flags;

    virtual void Update() {};
    virtual void CleanUp() {};

    const bool getShow() {
        return show;
    }

    void toggleWindow(){
        show = !show;
    }

    void setShowWindow(const bool showWindow){
        show = showWindow;
    }


};

