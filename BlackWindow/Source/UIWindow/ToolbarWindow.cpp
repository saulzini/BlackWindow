#include "ToolbarWindow.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "Core/Time/DeltaTime.h"
void ToolbarWindow::Update()
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

        if (ImGui::SmallButton("Play"))
        {
            App->deltaTime.SetTimeScale(1.0f);
        }
        ImGui::SameLine();


        if (ImGui::SmallButton("Pause"))
        {
            App->deltaTime.SetTimeScale(0.0f);
        }
        ImGui::SameLine();

        if (ImGui::SmallButton("Stop"))
        {
            App->deltaTime.SetTimeScale(0.0f);
            App->camera->ResetCamera();
        }

        end();
    }
}
