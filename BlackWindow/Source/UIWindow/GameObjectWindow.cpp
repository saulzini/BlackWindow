#include "GameObjectWindow.h"
#include "Math/float3.h"
#include "Application.h"
#include "ModuleScene.h"
// #include "Core/Model.h"
void GameObjectWindow::Update()
{
    if (show)
    {
        // model = App->scene->GetModel();
        
        ImGui::SetWindowPos(ImVec2(20, 30), true);
        if (!begin())
        {
            show = false;
            end();
            return;
        }

        // if (model ==nullptr){
        //     ImGui::Text("No Model");
        // }
        // else {
        //     DrawTransformation();
        //     DrawGeometry();
        //     DrawTexture();
        // }
        end();
    }
}

// void GameObjectWindow::DrawTransformation()
// {
//     if (ImGui::CollapsingHeader("Transformation"))
//     {
//         // Position
//         float3 position(0,0,0);
//         ImGui::InputFloat3("Front", &position[0]);
//         // App->camera->SetFrontVector(frontVector);

//         // Rotation
//         float3 rotation(0,0,0);
//         ImGui::InputFloat3("Up", &rotation[0]);
//         // App->camera->SetUpVector(upVector);

//         // Scale
//         float3 scale(0,0,0);
//         ImGui::InputFloat3("Position", &scale[0]);
//     }
// }

// void GameObjectWindow::DrawGeometry() 
// {
//     if (ImGui::CollapsingHeader("Geometry"))
//     {
//         ImGui::Text("Animations count %d", model->GetAnimationsCount());
//         ImGui::Text("Meshes count %d", model->GetMeshesCount());
//         ImGui::Text("Materials count %d", model->GetMaterialsCount());
//         ImGui::Text("Cameras count %d", model->GetCamerasCount());
//         ImGui::Text("Lights count %d", model->GetLightsCount());
//         ImGui::Text("Textures count %d", model->GetTexturesCount());
        
//         float3 dimensions =model->GetDimensions();
//         ImGui::TextWrapped("Bounding box dimensions", dimensions.x,dimensions.y,dimensions.z);
//         ImGui::TextWrapped("X: %f\nY: %f\nZ: %f", dimensions.x,dimensions.y,dimensions.z);
//     }
// }

// void GameObjectWindow::DrawTexture() 
// {
//     if (ImGui::CollapsingHeader("Texture"))
//     {

//         std::vector<Texture> textures = model->GetTexturesLoaded();
//         for (size_t i = 0; i < textures.size(); i++)
//         {
//             ImVec2 uvMin = ImVec2(0.0f, 0.0f);                 // Top-left
//             ImVec2 uvMax = ImVec2(1.0f, 1.0f);                 // Lower-right
//             ImVec4 tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
//             ImVec4 borderCol = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
//             ImVec2 sizeImageDisplay = ImVec2(100.0f, 100.0f);
//             ImGui::Image( (ImTextureID) textures[i].id, sizeImageDisplay, uvMin, uvMax, tintCol, borderCol);
//         }
        
//     }
// }
