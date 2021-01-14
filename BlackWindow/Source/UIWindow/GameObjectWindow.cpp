#include "GameObjectWindow.h"
#include "Math/float3.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Core/GameObject/GameObject.h"
#include "Core/Components/ComponentTransform.h"
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

        GameObject* gameObject = App->scene->GetSelected();
        if (!gameObject){
            end();
            return;
        }

        // Name
        //std::string name = gameObject->GetName();
        //ImGui::InputText("Name", &name[0], name.size()); //always pass chars and size
        //gameObject->SetName(name);
        std::vector<Component*> Components = gameObject->GetComponents();
        for (std::vector<Component*>::iterator it = Components.begin(); it != Components.end(); ++it) {
            (*it)->OnEditor();
        }
 

        end();
    }
}


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
