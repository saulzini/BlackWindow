#pragma once
#include "ComponentTypes.h"

class GameObject;
class Component
{
protected:
	ComponentTypes type; 
    bool active;
    GameObject *owner;

public:
    
    Component(GameObject *owner ,ComponentTypes type = ComponentTypes::NONE){
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

};

