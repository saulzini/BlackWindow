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

            ImGui::PushID(root->GetId());

            if (ImGui::IsItemClicked())
            {
                App->editor->consoleWindow->AddLog("Clicked %s", root->GetName().c_str());
                App->scene->SetSelected(root);
            }

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HierarchyNodeZone"))
                {
                    selected = App->scene->GetSelected();
                    IM_ASSERT(payload->DataSize == sizeof(GameObject*));
                    // GameObject* sourceId = (GameObject*)payload->Data;
                    App->editor->consoleWindow->AddLog("From %s to %s ", selected->GetName().c_str(), root->GetName().c_str());
                    selected->SetParent(root);
                }
                ImGui::EndDragDropTarget();
            }


            ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);
            std::vector<GameObject *> gameObjects = root->GetChildren();
            ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

            for (std::vector<GameObject *>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
            {
                GameObject *gameObject = (GameObject *)*it;
                if (gameObject->GetChildren().empty())
                {
                    nodeFlags = ImGuiTreeNodeFlags_Leaf;
                }
                selected = App->scene->GetSelected();
                if (selected){
                    App->editor->consoleWindow->AddLog("Selected %s current %s ", selected->GetName().c_str(), gameObject->GetName().c_str());

                }
                if (selected  && gameObject == selected){
                    App->editor->consoleWindow->AddLog("Selected %s same %s ", selected->GetName().c_str(), gameObject->GetName().c_str());

                    nodeFlags |= ImGuiTreeNodeFlags_Selected;
                }
                bool open = ImGui::TreeNodeEx((void *)gameObject->GetId(), nodeFlags, gameObject->GetName().c_str());
                TreeChildren(open, gameObject);
            }
            ImGui::PopID();
            ImGui::TreePop();
            ImGui::PopStyleVar();
        }

        end();
    }
}

void HierarchyWindow::TreeChildren(bool isOpen, GameObject *currentNode)
{
    if (isOpen)
    {
        ImGui::PushID(currentNode->GetId());

        if (ImGui::IsItemClicked())
        {
            App->editor->consoleWindow->AddLog("Clicked %s", currentNode->GetName().c_str());
            App->scene->SetSelected(currentNode);
        }

        // Our buttons are both drag sources and drag targets here!
        if (ImGui::BeginDragDropSource())
        {
            // Set payload to carry the index of our item (could be anything)
            ImGui::SetDragDropPayload("HierarchyNodeZone", &currentNode, sizeof(GameObject *));

            // Display preview (could be anything, e.g. when dragging an image we could decide to display
            // the filename and a small preview of the image, etc.)
            ImGui::Text("Swap %s", currentNode->GetName().c_str());
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("HierarchyNodeZone"))
            {
                selected = App->scene->GetSelected();
                IM_ASSERT(payload->DataSize == sizeof(GameObject *));
                App->editor->consoleWindow->AddLog("From %s to %s ",  selected->GetName().c_str(), currentNode->GetName().c_str());
                // checking if is child
                if ( !selected->isChild(currentNode)){
                    selected->SetParent(currentNode);
                }
            }
            ImGui::EndDragDropTarget();
        }

        if (!currentNode->GetChildren().empty())
        {
            std::vector<GameObject *> gameObjects = currentNode->GetChildren();
            for (std::vector<GameObject *>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
            {
                GameObject *gameObject = (GameObject *)*it;
            	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

                if (gameObject->GetChildren().empty())
                {
                    flags = ImGuiTreeNodeFlags_Leaf;
                }
                selected = App->scene->GetSelected();

                if (selected != nullptr && gameObject == selected){
                    flags |= ImGuiTreeNodeFlags_Selected;
                }
                bool open = ImGui::TreeNodeEx((void *)gameObject->GetId(), flags, gameObject->GetName().c_str());
                TreeChildren(open, gameObject);
            }
        }
        ImGui::PopID();
        ImGui::TreePop();

    }
}
