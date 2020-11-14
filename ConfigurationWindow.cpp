#include "ConfigurationWindow.h"

void ConfigurationWindow::Update()
{
    if (show)
    {
        if (!begin())
        {
            show = false;
            end();
            return;
        }

        ImGui::Text(
            "Console window ");
        end();
    }
}
