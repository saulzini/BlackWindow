#pragma once
#include "ComponentTypes.h"
#include "json/json.h"

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
    virtual void Init() {}
    virtual void Enable(){}
    virtual void Update(){}
    virtual void Disable(){}
    virtual void OnEditor(){}
    virtual void OnSave(Json::Value& owner){}
    virtual void OnLoad(const Json::Value& componentJson){}
    virtual void Clear(){}

};

