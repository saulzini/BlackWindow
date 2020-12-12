#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"

class ComponentMesh : public Component
{
public:
    ComponentMesh(ComponentTypes type = ComponentTypes::MESH) : Component(type){};
};

