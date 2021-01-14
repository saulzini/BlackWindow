#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include <Math/float3.h>
#include "imgui.h"
#include "Math/Quat.h"
#include <vector>

class ComponentMaterial : public Component
{
public:
    ComponentMaterial(GameObject *owner = nullptr, ComponentTypes type = ComponentTypes::MATERIAL) : Component(owner, type){};

    void OnEditor()
    {
        // float3 auxPosition = GetPosition();
        // float3 auxScale = GetScale();
        // float3 auxRotation = GetRotation();
        // if (ImGui::CollapsingHeader("Transformation"))
        // {
        //     ImGui::InputFloat3("Position", &auxPosition[0]);
        //     ImGui::InputFloat3("Scale", &auxScale[0]);
        //     ImGui::InputFloat3("Rotation", &auxRotation[0]);
        // }
        // SetPosition(auxPosition);
        // SetScale(auxScale);
        // SetRotation(auxRotation);
    }

    void OnSave(Json::Value &parent) override
    {
        // Json::Value transformJson;
        // transformJson["type"] = static_cast<int>(ComponentTypes::TRANSFORM);

        // Json::Value positionJson = Json::arrayValue;
        // Json::Value scaleJson = Json::arrayValue;
        // Json::Value rotationJson = Json::arrayValue;

        // positionJson.append(position.x);
        // positionJson.append(position.y);
        // positionJson.append(position.z);

        // scaleJson.append(scale.x);
        // scaleJson.append(scale.y);
        // scaleJson.append(scale.z);

        // rotationJson.append(rotation.x);
        // rotationJson.append(rotation.y);
        // rotationJson.append(rotation.z);

        // transformJson["position"] = positionJson;
        // transformJson["scale"] = scaleJson;
        // transformJson["rotation"] = rotationJson;

        // parent["components"].append(transformJson);
    }

    void OnLoad(const Json::Value &componentJson) override
    {
    }

    void setTextureId(const unsigned int textureId)
    {
        this->textureId = textureId;
    }

    void SetSpecularId(const unsigned int specularId)
    {
        this->specularId = specularId;
    }

    unsigned int GetTextureId()
    {
        return textureId;
    }

    unsigned int GetSpecularId()
    {
        return specularId;
    }

private:
    unsigned int textureId;
    unsigned int specularId;
};
