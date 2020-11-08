#include "ConsoleWindow.h"
#include "ImGui/imgui.h"


void ConsoleWindow::update()
{
    if (show)
    {
        if (!begin())
        {
            show = false;
            end();
        }
        else
        {
            ImGui::Text("Hello from another window!");
            end();
        }
    }
}
