#pragma once
#include <string>
#include <vector>
#include "Core/Components/ComponentTypes.h"
#include "Math/float4x4.h"
#include "Application.h"
#include "Core/Components/ComponentTransform.h"
#include "Core/Components/Component.h"
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
    }
    ~GameObject() {}
    virtual void Update();
    virtual void Draw();

    std::vector<GameObject *> GetChildren()
    {
        return children;
    }

    Component* AddComponent(ComponentTypes type);
    void AddComponent(Component * component);

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

    
    void RemoveChild(const GameObject *child)
    {
        if (children.empty() )
        {
            return;
        }
        children.erase(std::remove(children.begin(), children.end(), child), children.end());
    }

    void SetParent(GameObject *newParent)
    {
        // parent position
        float4x4 oldParentMatrix = float4x4::identity;
        float4x4 newParentMatrix = newParent->GetModelMatrix();

        // delete from parent
        if (parent != nullptr)
        {
            oldParentMatrix = parent->GetModelMatrix();
            parent->RemoveChild(this);
        }
        // asssigning new parent
        parent = newParent;
        parent->AddChildren(this);

        float3 translateOld(0.0f);
        Quat rotationOld(0.0f, 0.0f, 0.0f, 0.0f);
        float3 scaleOld(0.0f);

        float3 translateNew(0.0f);
        Quat rotationNew(0.0f, 0.0f, 0.0f, 0.0f);
        float3 scaleNew(0.0f);

        oldParentMatrix.Decompose(translateOld, rotationOld, scaleOld);
        newParentMatrix.Decompose(translateNew, rotationNew, scaleNew);

        Quat rotationCurrent = transformComponent->GetRotationQuat();

        float3 newRotation = float3(rotationOld.x + rotationCurrent.x - rotationNew.x,
                                    rotationOld.y + rotationCurrent.y - rotationNew.y,
                                    rotationOld.z + rotationCurrent.z - rotationNew.z);

        if (transformComponent)
        {
            transformComponent->SetPosition((translateOld + transformComponent->GetPosition()) - translateNew);
            transformComponent->SetScale((scaleOld + transformComponent->GetScale()) - scaleNew);
            transformComponent->SetRotation(newRotation);
        }
        CalculateModelMatrix();
    }

    void SetName(const std::string &name)
    {
        this->name = name;
    }

    ComponentTransform *GetTransformComponent() const
    {
        return transformComponent;
    }

    float4x4 GetProjectionMatrix() {
        return projectionMatrix;
    }

    float4x4 GetViewMatrix() {
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

    void RemoveParent(){
        if (parent == nullptr){
            return;
        }
        // removing from parent
        if (parent != nullptr){
            parent->RemoveChild(this);
        }
        // removing relation
        parent = nullptr;
    }

    void Clear()
    {
        // Clear components
        if (!components.empty())
        {
            for (std::vector<Component *>::iterator it = components.begin(); it != components.end(); ++it){
                ( (Component *) *it )->Clear();
            }
            components.clear();
        }
    }

};
