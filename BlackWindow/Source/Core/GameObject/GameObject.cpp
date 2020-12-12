#pragma once
#include "GameObject.h"
#include "Core/Components/Component.h"

Component* GameObject::CreateComponent(ComponentTypes type)
{
    return new Component(type);
}
