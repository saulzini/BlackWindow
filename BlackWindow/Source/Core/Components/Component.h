#pragma once
#include "ComponentTypes.h"
//#include "Core/GameObject/GameObject.h"
//#include "Core/GameObject/GameObject.h"
class GameObject;
class Component
{
protected:
	ComponentTypes type; 
    bool active;
    GameObject *owner;

public:
    
    Component(GameObject *owner = nullptr,ComponentTypes type = ComponentTypes::NONE){
        this->owner = owner;
        this->type = type;
        active = true;
    }

    ~Component() {}

    ComponentTypes GetType() const{
        return type;
    }
    virtual void Enable(){}
    virtual void Update(){}
    virtual void Disable(){}
    virtual void OnEditor(){}
    virtual void OnSave(){}
    virtual void OnLoad(){}

};

