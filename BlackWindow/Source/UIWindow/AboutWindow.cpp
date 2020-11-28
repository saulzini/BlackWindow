#include "AboutWindow.h"

void AboutWindow::Update()
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

        ImGui::Text("Engine: Black Window");
        ImGui::Text("Description: Engine");
        ImGui::Text("Authors: Saul Juarez");
        ImGui::Text("Libraries: Assimp, Devil, Glew, Gpu detect, Imgui, SDL,MathGeoLib");
        ImGui::Text("License: MIT");

        end();
    }
}
