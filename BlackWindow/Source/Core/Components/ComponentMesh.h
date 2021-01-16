#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include "Core/Vertex.h"
#include <vector>
class ComponentMesh : public Component
{

public:
    ComponentMesh(GameObject *owner = nullptr, ComponentTypes type = ComponentTypes::MESH) : Component(owner, type) {}

    void SetVertices(const std::vector<Vertex> &vertices)
    {
        this->vertices = vertices;
    }

    void OnSave(Json::Value &owner) override
    {
        Json::Value componentJson;
        componentJson["type"] = static_cast<int>(ComponentTypes::MESH);

        for (std::vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); ++it)
        {
            (*it).Save(componentJson);
        }

        componentJson["indices"] = Json::arrayValue;
        for (std::vector<unsigned int >::iterator it = indices.begin(); it != indices.end(); ++it)
        {
            Json::UInt index = (*it);
            componentJson["indices"].append(index);
        }

        owner["components"].append(componentJson);

    }

    void OnEditor() override
    {
        if (ImGui::CollapsingHeader("Mesh"))
        {
            ImGui::Text("Mesh vertices %d",vertices.size());
            ImGui::Text("Mesh indices %d",indices.size());
        }
    }

    void Clear() override
    {
        vertices.clear();
        indices.clear();
    }

    void OnLoad(const Json::Value &componentJson) override
    {
        // Clear();

        for(unsigned int i=0; i< componentJson["vertices"].size(); i++ ){
            Vertex vertex;
            vertex.LoadFromJson(componentJson["vertices"][i]);
            vertices.push_back(vertex);
        }
        for(unsigned int i=0; i< componentJson["indices"].size(); i++ ){
            indices.push_back( componentJson["indices"][i].asUInt() );
        }

    }

    std::vector<Vertex> GetVertices() const
    {
        return vertices;
    }

    void SetIndices(const std::vector<unsigned int> &indices)
    {
        this->indices =indices;
    }

    std::vector<unsigned int> GetIndices() const
    {
        return indices;
    }

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

};
