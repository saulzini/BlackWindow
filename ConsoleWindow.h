#pragma once

#include "DefaultImGuiWindow.h"
#include "ImGui/imgui.h"
class ConsoleWindow :
	public DefaultImGuiWindow
{
public:
	ConsoleWindow(const char* _title) : DefaultImGuiWindow(_title) {
	}
    ~ConsoleWindow();
	void update() override;
    char                  InputBuf[256];
    ImVector<char*>       Items;
    ImVector<const char*> Commands;
    ImVector<char*>       History;
    int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter       Filter;
    bool                  AutoScroll;
    bool                  ScrollToBottom;
};

