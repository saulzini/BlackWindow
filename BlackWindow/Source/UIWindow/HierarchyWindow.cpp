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

            selected = App->scene->GetSelected();

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HierarchyNodeZone"))
                {
                    IM_ASSERT(payload->DataSize == sizeof(GameObject*));
                    GameObject* sourceId = (GameObject*)payload->Data;

                    App->editor->consoleWindow->AddLog("Dragged %s to %i ", root->GetName().c_str(), sourceId);
                }
                ImGui::EndDragDropTarget();
            }


            ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);
            std::vector<GameObject *> gameObjects = root->GetChildren();
            ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

            for (std::vector<GameObject *>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
            {
                GameObject *gameObject = (GameObject *)*it;
                if (selected != nullptr && gameObject == selected){
                    nodeFlags |= ImGuiTreeNodeFlags_Selected;
                }
                bool open = ImGui::TreeNodeEx((void *)gameObject->GetId(), nodeFlags, gameObject->GetName().c_str());
                TreeChildren(nodeFlags, open, gameObject);
            }
            ImGui::TreePop();
            ImGui::PopStyleVar();
        }

        end();
    }
}

void HierarchyWindow::TreeChildren(ImGuiTreeNodeFlags nodeFlags, bool isOpen, GameObject *currentNode)
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
                IM_ASSERT(payload->DataSize == sizeof(GameObject *));
                GameObject *sourceId = (GameObject *)payload->Data;

                App->editor->consoleWindow->AddLog("From %i to %s ",  sourceId, currentNode->GetName().c_str());
            }
            ImGui::EndDragDropTarget();
        }

        if (!currentNode->GetChildren().empty())
        {
            std::vector<GameObject *> gameObjects = currentNode->GetChildren();
            for (std::vector<GameObject *>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
            {
                GameObject *gameObject = (GameObject *)*it;
            	ImGuiTreeNodeFlags flags = originalNodeFlags;

                if (gameObject->GetChildren().size() <= 0)
                {
                    flags |= ImGuiTreeNodeFlags_Leaf;
                }
                if (selected != nullptr && gameObject == selected){
                    flags |= ImGuiTreeNodeFlags_Selected;
                }
                bool open = ImGui::TreeNodeEx((void *)gameObject->GetId(), flags, gameObject->GetName().c_str());
                TreeChildren(flags, open, gameObject);
            }
        }
        ImGui::PopID();
        ImGui::TreePop();

    }
}
