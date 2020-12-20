#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include <Math/float3.h>

class ComponentTransform : public Component
{
public:
    ComponentTransform(GameObject *owner , ComponentTypes type = ComponentTypes::TRANSFORM) : Component(owner,type){};
    
    void SetTransform(float3 newTransform)
    {
        transform = newTransform;
    }

    float3 GetTransform() const 
    {
        return transform;
    }

    void SetScale(float3 newScale)
    {
        scale = newScale;
    }

    float3 GetScale() const
    {
        return transform;
    }

    void SetRotation(float3 newRotation)
    {
        rotation = newRotation;
    }

    float3 GetRotation() const
    {
        return rotation;
    }


protected:
    float3 transform;
    float3 scale;
    float3 rotation;
};

