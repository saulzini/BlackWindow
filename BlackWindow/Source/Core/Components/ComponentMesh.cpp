#include "Core/Components/ComponentMesh.h"
#include "imgui.h"

void ComponentMesh::OnSave(Json::Value &owner)
{
    Json::Value componentJson;
    componentJson["type"] = static_cast<int>(ComponentTypes::MESH);

    for (std::vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        (*it).Save(componentJson);
    }

    componentJson["indices"] = Json::arrayValue;
    for (std::vector<unsigned int>::iterator it = indices.begin(); it != indices.end(); ++it)
    {
        Json::UInt index = (*it);
        componentJson["indices"].append(index);
    }

    owner["components"].append(componentJson);
}

void ComponentMesh::OnEditor()
{
    if (ImGui::CollapsingHeader("Mesh"))
    {
        ImGui::Text("Mesh vertices %d", vertices.size());
        ImGui::Text("Mesh indices %d", indices.size());
    }
}

void ComponentMesh::Clear()
{
    vertices.clear();
    indices.clear();
}

void ComponentMesh::OnLoad(const Json::Value &componentJson)
{
    for (unsigned int i = 0; i < componentJson["vertices"].size(); i++)
    {
        Vertex vertex;
        vertex.LoadFromJson(componentJson["vertices"][i]);
        vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < componentJson["indices"].size(); i++)
    {
        indices.push_back(componentJson["indices"][i].asUInt());
    }
}