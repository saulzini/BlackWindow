#pragma once

#include "DefaultImGuiWindow.h"
#include "imgui.h"

class ConsoleWindow :
    public DefaultImGuiWindow
{
public:
    ConsoleWindow(
        const char* _title, 
        ImGuiWindowFlags _window_flags ) : DefaultImGuiWindow(_title, _window_flags) 
    {
        ClearLog();
        memset(InputBuf, 0, sizeof(InputBuf));
        HistoryPos = -1;
        // "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
        AutoScroll = true;
        ScrollToBottom = false;
    };
    ~ConsoleWindow();
    void Update() override;
    void ClearLog();
    void AddLog(const char* fmt, ...) IM_FMTARGS(2);

private:
    char                  InputBuf[256];
    ImVector<char*>       Items;
    ImVector<const char*> Commands;
    ImVector<char*>       History;
    int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter       Filter;
    bool                  AutoScroll;
    bool                  ScrollToBottom;

};

