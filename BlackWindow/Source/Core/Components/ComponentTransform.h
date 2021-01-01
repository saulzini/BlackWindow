#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include <Math/float3.h>
#include "imgui.h"
#include "Math/Quat.h"
//#include "Core/GameObject/GameObject.h"

class ComponentTransform : public Component
{
public:
    ComponentTransform(GameObject *owner = nullptr, ComponentTypes type = ComponentTypes::TRANSFORM) : Component(owner,type){};
    
    void SetPosition(float3 newPosition)
    {
        if (position.Equals(newPosition)){
            return;
        }
        position = newPosition;
       // this->owner->CalculateModelMatrix();
    }

    float3 GetPosition() const 
    {
        return position;
    }

    void SetScale(float3 newScale)
    {
        if (scale.Equals(newScale)){
            return;
        }
        scale = newScale;
        // owner->CalculateModelMatrix();
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
        // owner->CalculateModelMatrix();
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
        float3 auxPosition = GetPosition();
        float3 auxScale = GetScale();
        float3 auxRotation = GetRotation();
        if (ImGui::CollapsingHeader("Transformation"))
        {
            ImGui::InputFloat3("Position", &auxPosition[0]);
            ImGui::InputFloat3("Scale", &auxScale[0]);
            ImGui::InputFloat3("Rotation", &auxRotation[0]);
        }
        SetPosition(auxPosition);
        SetScale(auxScale);
        SetRotation(auxRotation);

    }

    void OnSave(Json::Value& parent) override
    {
        
    }

protected:
    float3 position;
    float3 scale;
    float3 rotation;
    Quat rotationQuat;
};

