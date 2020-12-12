#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"

class ComponentTransform : public Component
{
public:
    
    ComponentTransform(ComponentTypes type = ComponentTypes::NONE) : Component(type){};
};

