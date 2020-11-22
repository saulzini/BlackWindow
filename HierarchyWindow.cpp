#include "HierarchyWindow.h"

void HierarchyWindow::Update()
{
    if (show)
    {
        if (!begin())
        {
            show = false;
            end();
            return;
        }

        ImGui::Text("Hierarchy window");
        end();
    }
}
