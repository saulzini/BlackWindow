#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include "Core/Components/ComponentMesh.h"
#include "Core/Components/ComponentTransform.h"
#include "Core/Components/ComponentLight.h"
#include "Core/Components/ComponentCamera.h"
#include "Core/Components/ComponentMaterial.h"
#include "Core/Components/ComponentMeshRenderer.h"
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
            return new ComponentMaterial(owner, type);
        case ComponentTypes::LIGHT:
            return new ComponentLight(owner, type);
        case ComponentTypes::CAMERA:
            return new ComponentCamera(owner, type);
        case ComponentTypes::MESHRENDERER:
            return new ComponentMeshRenderer(owner, type);
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