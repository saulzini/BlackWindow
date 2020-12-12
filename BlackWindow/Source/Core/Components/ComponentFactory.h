#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
namespace ComponentFactory
{
    static Component* CreateComponent(ComponentTypes type){
        switch (type)
        {
            default:
                return new Component(type); 
        }
    }
}