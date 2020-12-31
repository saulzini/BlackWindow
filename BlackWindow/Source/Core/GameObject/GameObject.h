#pragma once
#include <string>
#include <vector>
#include "Core/Components/ComponentTypes.h"
#include "Math/float4x4.h"
#include "Application.h"
#include "Core/Components/ComponentTransform.h"
#include "Core/Components/Component.h"
// class Component;
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
    std::vector<GameObject*> children;
    std::vector<Component*> components;
    ComponentTransform *transformComponent;
public:
    GameObject(GameObject* parent = nullptr, const char* name = "",unsigned int program=0){
        this->parent = parent;
        this->name = name;
        this->program = program;
        
        this->id = App->GetLcg()->Int();
        projectionMatrix = float4x4::identity;
        viewMatrix = float4x4::identity;
        modelMatrix = float4x4::identity;

        this->transformComponent = nullptr;
    }
    ~GameObject(){}
    virtual void Update();
    virtual void Draw();

    std::vector<GameObject*> GetChildren() {
        return children;
    }

    Component* AddComponent(ComponentTypes type);
    void AddChildren(GameObject* gameObject);
    void SetProjectionMatrix(const float4x4 &projection);
    void SetViewMatrix(const float4x4 &projection);
    
    std::string GetName() const{
        return name;
    }

    int GetId() const{
        return id;
    }

    void RemoveChild(const GameObject* child){
        if (children.size() <= 0){
            return;
        }
        children.erase(std::remove(children.begin(), children.end(), child), children.end());
    }

    void SetParent(GameObject *newParent){
        // delete from parent
        if (parent != nullptr){
            parent->RemoveChild(this);
        }
        // asssigning new parent
        parent = newParent;
        parent->AddChildren(this);
    }

    void SetName(const std::string name){
        this->name = name;
    }

    ComponentTransform* GetTransformComponent() const{
        return transformComponent;
    }

    void CalculateModelMatrix();

    bool isChild(GameObject *lookingChild);

    void Save();
    void Export();
};
