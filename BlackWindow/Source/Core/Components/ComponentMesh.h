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

    void OnSave(Json::Value &owner) override;

    void OnEditor() override;

    void Clear() override;

    void OnLoad(const Json::Value &componentJson) override;

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
