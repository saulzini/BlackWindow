#pragma once
#include <string>
#include <vector>
#include "Core/Components/ComponentTypes.h"

class Component;
class GameObject
{
protected:
    std::string name;
    std::vector<GameObject*> children;
    std::vector<Component*> components;
    GameObject *parent;
public:
    GameObject(GameObject* parent = nullptr, const char* name = ""){
        this->name = name;
        this->parent = parent;
    }
    ~GameObject(){}
    virtual void Update();

    Component* AddComponent(ComponentTypes type);
    void AddChildren(GameObject* gameObject);
    
};
