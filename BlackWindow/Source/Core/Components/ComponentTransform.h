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
        Json::Value transformJson;
        transformJson["type"] = static_cast<int>(ComponentTypes::TRANSFORM);

        Json::Value positionJson = Json::arrayValue;
        Json::Value scaleJson = Json::arrayValue;
        Json::Value rotationJson = Json::arrayValue;

        positionJson.append( position.x );
        positionJson.append( position.y );
        positionJson.append( position.z );

        scaleJson.append( scale.x );
        scaleJson.append( scale.y );
        scaleJson.append( scale.z );

        rotationJson.append( rotation.x );
        rotationJson.append( rotation.y );
        rotationJson.append( rotation.z );

        transformJson["position"] = positionJson;
        transformJson["scale"] = scaleJson;
        transformJson["rotation"] = rotationJson;

        parent["components"].append(transformJson);
    }

    void OnLoad(const Json::Value& jRoot) override
    {
        SetPosition( float3( jRoot["position"][0].asFloat() , jRoot["position"][1].asFloat() , jRoot["position"][2].asFloat() ) );
        SetScale( float3( jRoot["scale"][0].asFloat() , jRoot["scale"][1].asFloat() , jRoot["scale"][2].asFloat() ) );
        SetRotation( float3( jRoot["rotation"][0].asFloat() , jRoot["rotation"][1].asFloat() , jRoot["rotation"][2].asFloat() ) );
    }
    

protected:
    float3 position;
    float3 scale;
    float3 rotation;
    Quat rotationQuat;
};

