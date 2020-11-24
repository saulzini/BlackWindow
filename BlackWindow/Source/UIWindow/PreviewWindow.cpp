#include "PreviewWindow.h"

void PreviewWindow::Update()
{
    if (show)
    {
        ImGui::SetNextWindowContentSize(ImVec2(400, 70.0f));

        if (!begin())
        {
            show = false;
            end();
            return;
        }

        end();
    }
}
