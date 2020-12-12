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
    GameObject(){
        name = "";
        parent = nullptr;
    }
    ~GameObject(){}
    virtual void Update(){}
    void AddComponent(ComponentTypes type);
};
