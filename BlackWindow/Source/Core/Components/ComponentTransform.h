#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include <Math/float3.h>
#include "imgui.h"
#include "Math/Quat.h"

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
        return scale;
    }

    void SetRotation(float3 newRotation)
    {
        if (rotation.Equals(newRotation )){
            return;
        }
        rotation = newRotation;
        rotationQuat = Quat::FromEulerXYX(newRotation.x,newRotation.y,newRotation.z);
    }

    float3 GetRotation() const
    {
        return rotation;
    }

    Quat GetRotationQuat() const
    {
        return rotationQuat;
    }

    void OnEditor()
    {
        float3 auxRotation = GetRotation();
        if (ImGui::CollapsingHeader("Transformation"))
        {
            ImGui::InputFloat3("Transform", &transform[0]);
            ImGui::InputFloat3("Scale", &scale[0]);
            ImGui::InputFloat3("Rotation", &auxRotation[0]);
        }
        SetRotation(auxRotation);

    }

protected:
    float3 transform;
    float3 scale;
    float3 rotation;
    Quat rotationQuat;
};

