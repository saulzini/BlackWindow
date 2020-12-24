#include "HierarchyWindow.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Core/GameObject/GameObject.h"
#include <iterator>
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

        if (ImGui::TreeNode("Trees"))
        {
            if (ImGui::TreeNode("Basic trees"))
            {
                for (int i = 0; i < 5; i++)
                {
                    // Use SetNextItemOpen() so set the default state of a node to be open. We could
                    // also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
                    if (i == 0)
                        ImGui::SetNextItemOpen(true, ImGuiCond_Once);

                    if (ImGui::TreeNode((void *)(intptr_t)i, "Child %d", i))
                    {
                        ImGui::Text("blah blah");
                        ImGui::SameLine();
                        if (ImGui::SmallButton("button"))
                        {
                        }
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Advanced, with Selectable nodes"))
            {

                static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
                static bool align_label_with_current_x_position = false;
                static bool test_drag_and_drop = true;
                ImGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnArrow", (unsigned int *)&base_flags, ImGuiTreeNodeFlags_OpenOnArrow);
                ImGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnDoubleClick", (unsigned int *)&base_flags, ImGuiTreeNodeFlags_OpenOnDoubleClick);
                // ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanAvailWidth",    (unsigned int*)&base_flags, ImGuiTreeNodeFlags_SpanAvailWidth); ImGui::SameLine(); HelpMarker("Extend hit area to all available width instead of allowing more items to be laid out after the node.");
                ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanFullWidth", (unsigned int *)&base_flags, ImGuiTreeNodeFlags_SpanFullWidth);
                ImGui::Checkbox("Align label with current X position", &align_label_with_current_x_position);
                ImGui::Checkbox("Test tree node as drag source", &test_drag_and_drop);
                ImGui::Text("Hello!");
                if (align_label_with_current_x_position)
                    ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

                // 'selection_mask' is dumb representation of what may be user-side selection state.
                //  You may retain selection state inside or outside your objects in whatever format you see fit.
                // 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
                /// of the loop. May be a pointer to your own node type, etc.
                static int selection_mask = (1 << 2);
                int node_clicked = -1;
                for (int i = 0; i < 6; i++)
                {
                    // Disable the default "open on single-click behavior" + set Selected flag according to our selection.
                    ImGuiTreeNodeFlags node_flags = base_flags;
                    const bool is_selected = (selection_mask & (1 << i)) != 0;
                    if (is_selected)
                        node_flags |= ImGuiTreeNodeFlags_Selected;
                    if (i < 3)
                    {
                        // Items 0..2 are Tree Node
                        bool node_open = ImGui::TreeNodeEx((void *)(intptr_t)i, node_flags, "Selectable Node %d", i);
                        if (ImGui::IsItemClicked())
                            node_clicked = i;
                        if (test_drag_and_drop && ImGui::BeginDragDropSource())
                        {
                            ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                            ImGui::Text("This is a drag and drop source");
                            ImGui::EndDragDropSource();
                        }
                        if (node_open)
                        {
                            ImGui::BulletText("Blah blah\nBlah Blah");
                            ImGui::TreePop();
                        }
                    }
                    else
                    {
                        // Items 3..5 are Tree Leaves
                        // The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
                        // use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
                        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                        ImGui::TreeNodeEx((void *)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
                        if (ImGui::IsItemClicked())
                            node_clicked = i;
                        if (test_drag_and_drop && ImGui::BeginDragDropSource())
                        {
                            ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                            ImGui::Text("This is a drag and drop source");
                            ImGui::EndDragDropSource();
                        }
                    }
                }
                if (node_clicked != -1)
                {
                    // Update selection state
                    // (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
                    if (ImGui::GetIO().KeyCtrl)
                        selection_mask ^= (1 << node_clicked); // CTRL+click to toggle
                    else                                       //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
                        selection_mask = (1 << node_clicked);  // Click to single-select
                }
                if (align_label_with_current_x_position)
                    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Scene"))
        {
            // building tree
            GameObject *root = App->scene->GetRoot();
            if (root == nullptr)
            {
                return;
            }

            ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);
            std::vector<GameObject *> gameobjects = root->GetChildren();
            ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

            for (int i = 0; i < gameobjects.size(); i++)
            {
                bool node_open = ImGui::TreeNodeEx((void *)(intptr_t)i, node_flags, gameobjects[i]->GetName().c_str());

                TreeChildren(node_flags, node_open, gameobjects[i] );
            }
            ImGui::TreePop();
            ImGui::PopStyleVar();
        }

        end();
    }
}

void HierarchyWindow::TreeChildren(ImGuiTreeNodeFlags node_flags, bool isOpen,GameObject *currentNode )
{
    if (isOpen)
    {
        std::vector<GameObject *> gameobjects = currentNode->GetChildren();

        for (int i = 0; i < gameobjects.size(); i++)
        {
            if (gameobjects[i]->GetChildren().empty())
            {
                node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                ImGui::TreeNodeEx((void *)(intptr_t)gameobjects[i], node_flags, gameobjects[i]->GetName().c_str());

                if (ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                    ImGui::Text("This is a drag and drop source");
                    ImGui::EndDragDropSource();

                }
            }
            else
            {
                bool o = ImGui::TreeNodeEx((void *)(intptr_t)gameobjects[i], node_flags, gameobjects[i]->GetName().c_str());
                ImGui::TreePop();
                TreeChildren(node_flags, o, gameobjects[i]);
            }
        }
        ImGui::TreePop();
    }
}