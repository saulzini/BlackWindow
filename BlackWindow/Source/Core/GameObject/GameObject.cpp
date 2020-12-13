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
    for (unsigned int i = 0; i < components.size(); i++)
	{
        if (components[i]->GetType() == ComponentTypes::MESH){
		    components[i]->Update();
        }
	}

    for (unsigned int i = 0; i < children.size(); i++)
	{
        children[i]->Update();
	}
}
