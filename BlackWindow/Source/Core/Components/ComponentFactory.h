#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include "Core/Components/ComponentMesh.h"
#include "Core/Components/ComponentTransform.h"
namespace ComponentFactory
{
    static Component *CreateComponent(GameObject *owner, ComponentTypes type)
    {
        switch (type)
        {
        case ComponentTypes::MESH:
            return new ComponentMesh(owner, type);
        case ComponentTypes::TRANSFORM:
            return new ComponentTransform(owner, type);
        default:
            return new Component(owner, type);
        }
    }

    static void CreateComponentFromJson(const Json::Value &jRoot, GameObject *owner)
    {
        ComponentTypes componentType = static_cast<ComponentTypes>(jRoot["type"].asInt());
        Component *component = ComponentFactory::CreateComponent(owner, componentType);
        component->OnLoad(jRoot);
    }
} // namespace ComponentFactory