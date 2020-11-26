#include "GameObjectWindow.h"
#include "Math/float3.h"
#include "Application.h"
#include "ModuleWorld.h"
#include "Core/Model.h"
void GameObjectWindow::Update()
{
    if (show)
    {
       
        ImGui::SetWindowPos(ImVec2(20, 30), true);
        if (!begin())
        {
            show = false;
            end();
            return;
        }

        DrawTransformation();
        DrawGeometry();
        DrawTexture();

        end();
    }
}

void GameObjectWindow::DrawTransformation()
{
    if (ImGui::CollapsingHeader("Transformation"))
    {
        // Position
        float3 position(0,0,0);
        ImGui::InputFloat3("Front", &position[0]);
        // App->camera->SetFrontVector(frontVector);

        // Rotation
        float3 rotation(0,0,0);
        ImGui::InputFloat3("Up", &rotation[0]);
        // App->camera->SetUpVector(upVector);

        // Scale
        float3 scale(0,0,0);
        ImGui::InputFloat3("Position", &scale[0]);
    }
}

void GameObjectWindow::DrawGeometry() 
{
    if (ImGui::CollapsingHeader("Geometry"))
    {
        // Position
        float3 position(0,0,0);
        ImGui::InputFloat3("Front", &position[0]);
        // App->camera->SetFrontVector(frontVector);

        // Rotation
        float3 rotation(0,0,0);
        ImGui::InputFloat3("Up", &rotation[0]);
        // App->camera->SetUpVector(upVector);

        // Scale
        float3 scale(0,0,0);
        ImGui::InputFloat3("Position", &scale[0]);
    }
}

void GameObjectWindow::DrawTexture() 
{
    Model* model = App->world->GetModel();

    if (model ==nullptr){
        ImGui::Text("No Model");
        return;
    }
    if (ImGui::CollapsingHeader("Texture"))
    {

        std::vector<Texture> textures = model->GetTexturesLoaded();
        for (size_t i = 0; i < textures.size(); i++)
        {
            ImVec2 uvMin = ImVec2(0.0f, 0.0f);                 // Top-left
            ImVec2 uvMax = ImVec2(1.0f, 1.0f);                 // Lower-right
            ImVec4 tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
            ImVec4 borderCol = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
            ImVec2 sizeImageDisplay = ImVec2(100.0f, 100.0f);
            ImGui::Image( (ImTextureID) textures[i].id, sizeImageDisplay, uvMin, uvMax, tintCol, borderCol);
        }
        
    }
}
