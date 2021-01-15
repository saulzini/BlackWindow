#pragma once
#include <string>
#include <vector>
#include "Core/Components/ComponentTypes.h"
#include "Math/float4x4.h"
#include "Application.h"
#include "Core/Components/ComponentTransform.h"
#include "Core/Components/ComponentMaterial.h"
#include "Core/Components/ComponentMesh.h"
#include "json/json.h"

class GameObject
{
protected:
    GameObject *parent;
    std::string name;
    unsigned int program;
    int id;
    float4x4 projectionMatrix;
    float4x4 viewMatrix;
    float4x4 modelMatrix;
    std::vector<GameObject *> children;
    std::vector<Component *> components;
    ComponentMaterial * materialComponent;
    ComponentMesh *meshComponent;
    ComponentTransform *transformComponent;

public:
    GameObject(GameObject *parent = nullptr, const char *name = "", unsigned int program = 0)
    {
        this->parent = parent;
        this->name = name;
        this->program = program;

        this->id = App->GetLcg()->Int();
        projectionMatrix = float4x4::identity;
        viewMatrix = float4x4::identity;
        modelMatrix = float4x4::identity;

        this->transformComponent = nullptr;
        
        this->materialComponent = nullptr;
    }
    ~GameObject() {}
    virtual void Update();
    virtual void Draw();

    std::vector<GameObject *> GetChildren()
    {
        return children;
    }

    Component *AddComponent(ComponentTypes type);
    void AddComponent(Component *component);

    void AddChildren(GameObject *gameObject);
    void SetProjectionMatrix(const float4x4 &projection);
    void SetViewMatrix(const float4x4 &projection);

    std::vector<Component *> GetComponents()
    {
        return components;
    }

    std::string GetName() const
    {
        return name;
    }

    int GetId() const
    {
        return id;
    }

    void RemoveChild(const GameObject *child);

    void SetParent(GameObject *newParent);
    

    void SetName(const std::string &name)
    {
        this->name = name;
    }

    ComponentTransform *GetTransformComponent() const
    {
        return transformComponent;
    }

    ComponentMesh *GetMeshComponent() const
    {
        return meshComponent;
    }

    float4x4 GetProjectionMatrix()
    {
        return projectionMatrix;
    }

    float4x4 GetViewMatrix()
    {
        return viewMatrix;
    }

    void CalculateModelMatrix();

    bool isChild(GameObject *lookingChild);

    void Save();
    void Export(Json::Value &parent);

    float4x4 GetModelMatrix() const
    {
        return modelMatrix;
    }

    void RemoveParent();

    void Clear();

    void CheckDefaultsComponents(Component *component);

    ComponentMaterial * GetMaterialComponent() const
    {
        return materialComponent;
    }

    void ApplyTextureToModel(unsigned int textureId)
    {
        if(materialComponent){
            materialComponent->SetTexture(textureId);
        }
    }
};
