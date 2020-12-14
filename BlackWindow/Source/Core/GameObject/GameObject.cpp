#pragma once
#include "GameObject.h"
#include "Core/Components/ComponentFactory.h"
#include "Core/GameObject/GameObjectTypes.h"
Component* GameObject::AddComponent(ComponentTypes type)
{
    Component *component = ComponentFactory::CreateComponent(this,type);
    components.push_back( component );
    return component;
}

void GameObject::AddChildren(GameObject* gameObject) 
{
    children.push_back(gameObject);
}

void GameObject::Update() 
{
    
    for (std::vector<Component *>::iterator it = components.begin() ; it != components.end(); ++it){
        
		if ( ( (Component *) *it)->GetType() == ComponentTypes::MESH){
		    ( (Component *) *it)->Update();
        }
	}

    for (std::vector<GameObject *>::iterator it = children.begin() ; it != children.end(); ++it){
        ( (GameObject *) *it)->Update();
	}

}
