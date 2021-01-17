#include "ToolbarWindow.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "Core/Time/DeltaTime.h"
#include "Core/SceneFileManager/SceneFileManager.h"
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
            SceneFileManager::SaveForPlayThread();
            timer = TIMERSTATE::RUNNING;
        }
        ImGui::SameLine();


        if (ImGui::SmallButton("Pause"))
        {
            App->deltaTime.SetTimeScale(0.0f);
            timer = TIMERSTATE::PAUSE;
        }
        ImGui::SameLine();

        if (ImGui::SmallButton("Stop"))
        {
            App->deltaTime.SetTimeScale(0.0f);
            SceneFileManager::LoadForPlay();
            timer = TIMERSTATE::STOP;
        }

        switch (timer)
        {
        case TIMERSTATE::STOP:
            ImGui::Text("Timer stop");
            break;
        case TIMERSTATE::PAUSE:
            ImGui::Text("Timer pause");
            break;
        default:
            ImGui::Text("Timer running");
            break;
        }

        end();
    }
}
