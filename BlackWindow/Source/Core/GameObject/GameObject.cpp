#pragma once
#include "GameObject.h"
#include "Core/Components/ComponentFactory.h"

void GameObject::AddComponent(ComponentTypes type)
{
    components.push_back( ComponentFactory::CreateComponent(type) );
}
