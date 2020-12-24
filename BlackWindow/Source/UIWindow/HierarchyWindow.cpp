#include "HierarchyWindow.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Core/GameObject/GameObject.h"
// #include <iterator>
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
#include <iostream> //cout
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

        if (ImGui::TreeNode("Scene"))
        {
            // building tree
            GameObject *root = App->scene->GetRoot();
            if (root == nullptr)
            {
                return;
            }

            ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);
            std::vector<GameObject *> gameObjects = root->GetChildren();
            ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

            for (int i = 0; i < gameObjects.size(); i++)
            {
                bool nodeOpen = ImGui::TreeNodeEx((void *)(intptr_t)i, nodeFlags, gameObjects[i]->GetName().c_str());

                TreeChildren(nodeFlags, nodeOpen, gameObjects[i] );
            }
            ImGui::TreePop();
            ImGui::PopStyleVar();
        }

        end();
    }
}

void HierarchyWindow::TreeChildren(ImGuiTreeNodeFlags nodeFlags, bool isOpen,GameObject *currentNode )
{
    if (isOpen)
    {
        std::vector<GameObject*> gameObjects = currentNode->GetChildren();
        for (std::vector<GameObject*>::iterator it =  gameObjects.begin(); it != gameObjects.end(); ++it) 
        // for (int i = 0; i < gameObjects.size(); i++)
        {  
            GameObject* gameObject =  (GameObject*) *it;
            if ( ( (GameObject*) *it)->GetChildren().empty())
            {
                nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                int id = gameObject->GetId();
                ImGui::PushID(id);
                ImGui::TreeNodeEx((void *)(intptr_t) gameObject, nodeFlags, gameObject->GetName().c_str());

                if (ImGui::IsItemClicked()) {
                    // Some processing...
                    App->editor->consoleWindow->AddLog("Clicked %s",gameObject->GetName().c_str());
                }

                // Our buttons are both drag sources and drag targets here!
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    // Set payload to carry the index of our item (could be anything)
                    ImGui::SetDragDropPayload("HierarchyNodeZone", &id, sizeof(int));

                    // Display preview (could be anything, e.g. when dragging an image we could decide to display
                    // the filename and a small preview of the image, etc.)
                    ImGui::Text("Swap %s", gameObject->GetName().c_str());
                    ImGui::EndDragDropSource();
                }
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HierarchyNodeZone"))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(int));
                        int sourceId = *(const int*)payload->Data;

                        App->editor->consoleWindow->AddLog("Dragged %s to %i ", gameObject->GetName().c_str(),sourceId);

                    }
                    ImGui::EndDragDropTarget();
                }
                ImGui::PopID();
            }
            else
            {
                bool o = ImGui::TreeNodeEx((void *)(intptr_t) gameObject, nodeFlags, gameObject->GetName().c_str());
                ImGui::TreePop();
                TreeChildren(nodeFlags, o, gameObject);
            }
        }
        ImGui::TreePop();
    }
}