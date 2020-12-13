#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"

class ComponentTransform : public Component
{
public:
    ComponentTransform(GameObject *owner , ComponentTypes type = ComponentTypes::TRANSFORM) : Component(owner,type){};
};

