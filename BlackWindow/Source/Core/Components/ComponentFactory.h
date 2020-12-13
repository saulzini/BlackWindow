#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include "Core/Components/ComponentMesh.h"
namespace ComponentFactory
{
    static Component* CreateComponent(GameObject *owner ,ComponentTypes type){
        switch (type)
        {
            case ComponentTypes::MESH:
                return new ComponentMesh(owner,type);
            default:
                return new Component(owner,type);
        }
    }
}