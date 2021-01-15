#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include <Math/float3.h>
#include "imgui.h"
#include "Math/Quat.h"
#include <vector>
#include "Application.h"
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
class ComponentMaterial : public Component
{
public:
    ComponentMaterial(GameObject *owner = nullptr, ComponentTypes type = ComponentTypes::MATERIAL) : Component(owner, type){};

    void OnEditor() override
    {
        if (ImGui::CollapsingHeader("Texture"))
        {
            
            ImGui::Text("Diffuse");
            ImVec2 uvMin = ImVec2(0.0f, 0.0f);                 // Top-left
            ImVec2 uvMax = ImVec2(1.0f, 1.0f);                 // Lower-right
            ImVec4 tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
            ImVec4 borderCol = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
            ImVec2 sizeImageDisplay = ImVec2(100.0f, 100.0f);
            ImGui::Image( (ImTextureID) textureId, sizeImageDisplay, uvMin, uvMax, tintCol, borderCol);
            
            ImGui::Text("Specular");
            ImGui::Image( (ImTextureID) specularId, sizeImageDisplay, uvMin, uvMax, tintCol, borderCol);
        }
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

    void SetTexture(const unsigned int texture){
        if (textureId == 0){
            textureId = texture;
        }
        else if (specularId == 0) {
            specularId = texture;
        }
        else {
            App->editor->consoleWindow->AddLog("No available spaces on this material");
        }
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
