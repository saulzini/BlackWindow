#pragma once
#include <string>
#include <vector>
#include "Core/Components/ComponentTypes.h"
#include "Math/float4x4.h"
#include "Core/GameObject/GameObject.h"
class Component;
class GameObject
{
protected:
    GameObject *parent;
    std::string name;
    float4x4 projectionMatrix;
    float4x4 viewMatrix;
    float4x4 modelMatrix;
    unsigned int program;
    std::vector<GameObject*> children;
    std::vector<Component*> components;
public:
    GameObject(GameObject* parent = nullptr, const char* name = "",unsigned int program=0){
        this->parent = parent;
        this->name = name;
        this->program = program;
        projectionMatrix = float4x4::identity;
        viewMatrix = float4x4::identity;
        modelMatrix = float4x4::identity;
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
        return this->name;
    }

    
};
