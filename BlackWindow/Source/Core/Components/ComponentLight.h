#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include <Math/float3.h>
#include "imgui.h"
#include "debugdraw.h"
#include "Application.h"
#include "Math/Quat.h"
//#include "Core/GameObject/GameObject.h"

class ComponentLight : public Component
{
public:
    ComponentLight(GameObject* owner = nullptr, ComponentTypes type = ComponentTypes::LIGHT) : Component(owner, type) {};

    void SetPosition(float3 newPosition)
    {
        if (position.Equals(newPosition)) {
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
        if (scale.Equals(newScale)) {
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
        if (rotation.Equals(newRotation)) {
            return;
        }
        rotation = newRotation;
        rotationQuat = Quat::FromEulerXYX(newRotation.x, newRotation.y, newRotation.z);
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

        positionJson.append(position.x);
        positionJson.append(position.y);
        positionJson.append(position.z);

        scaleJson.append(scale.x);
        scaleJson.append(scale.y);
        scaleJson.append(scale.z);

        rotationJson.append(rotation.x);
        rotationJson.append(rotation.y);
        rotationJson.append(rotation.z);

        transformJson["position"] = positionJson;
        transformJson["scale"] = scaleJson;
        transformJson["rotation"] = rotationJson;

        parent["components"].append(transformJson);
    }

    void OnLoad(const Json::Value& componentJson) override
    {
        SetPosition(float3(componentJson["position"][0].asFloat(), componentJson["position"][1].asFloat(), componentJson["position"][2].asFloat()));
        SetScale(float3(componentJson["scale"][0].asFloat(), componentJson["scale"][1].asFloat(), componentJson["scale"][2].asFloat()));
        SetRotation(float3(componentJson["rotation"][0].asFloat(), componentJson["rotation"][1].asFloat(), componentJson["rotation"][2].asFloat()));
    }
    void Update() override {
        float3 arrowFrom = float3(1.0f,1.0f,1.0f);
        direction = float3(1.0f, 0.0f, 0.0f);
        float3 arrowTo = arrowFrom + direction;
        float radius = 0.5f;
        float3 offset;
        dd::arrow(arrowFrom, arrowTo, float3(1.0f, 1.0f, 1.0f), 1.0f);
        for (float angle = 0; angle <= 360; angle += 45)
        {
            offset = float3(radius * sin(angle), 0.0f, radius * cos(angle));
            //dd::arrow(arrowFrom + offset, arrowTo + offset, float3(1.0f, 1.0f, 1.0f), 1.0f);
        }
    }

protected:
    float3 position;
    float3 scale;
    float3 rotation;
    float3 direction;
    Quat rotationQuat;
};

#pragma once
