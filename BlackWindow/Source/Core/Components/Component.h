#pragma once
#include "ComponentTypes.h"

class GameObject;
class Component
{
private:
	ComponentTypes type; 
    bool active;
    GameObject *owner;

public:
    
    Component(ComponentTypes type = ComponentTypes::NONE){
        this->type = type;
        active = false;
        owner = nullptr;
    }

    ~Component() {}

    virtual void Enable(){}
    virtual void Update(){}
    virtual void Disable(){}

};

