#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include <Math/float3.h>
#include "imgui.h"
#include "Math/Quat.h"
#include "json/json.h"
class ComponentTransform : public Component
{
public:
    ComponentTransform(GameObject *owner = nullptr, ComponentTypes type = ComponentTypes::TRANSFORM) : Component(owner,type){
        position = float3(0.0f,0.0f,0.0f);
        scale = float3(1.0f,1.0f,1.0f);
        SetRotation(float3(0.0f,0.0f,0.0f));
    };
    
    void SetPosition(float3 newPosition)
    {
        if (position.Equals(newPosition)){
            return;
        }
        position = newPosition;
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

    void OnEditor() override;
    void OnSave(Json::Value& owner) override;
    void OnLoad(const Json::Value& componentJson) override;
    

protected:
    float3 position;
    float3 scale;
    float3 rotation;
    Quat rotationQuat;
};

