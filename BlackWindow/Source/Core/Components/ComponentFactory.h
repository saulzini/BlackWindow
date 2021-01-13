#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include "Core/Components/ComponentMesh.h"
#include "Core/Components/ComponentTransform.h"
#include "Core/Components/ComponentLight.h"

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
        case ComponentTypes::MATERIAL:
            return new ComponentTransform(owner, type);
        case ComponentTypes::LIGHT:
            return new ComponentLight(owner, type);
        default:
            return new Component(owner, type);
        }
    }

    static Component *CreateComponentFromJson(const Json::Value &componentJson, GameObject *owner)
    {
        ComponentTypes componentType = static_cast<ComponentTypes>(componentJson["type"].asInt());
        Component *component = ComponentFactory::CreateComponent(owner, componentType);
        component->OnLoad(componentJson);
        return component;
    }
} // namespace ComponentFactory