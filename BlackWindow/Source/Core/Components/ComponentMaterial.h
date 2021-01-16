#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include <Math/float3.h>
#include "imgui.h"
#include "Math/Quat.h"
#include <vector>
#include "Application.h"
#include "GL/glew.h"
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
#include "Core/Importers/Texture/TextureLoader.h"

class ComponentMaterial : public Component
{
public:
    ComponentMaterial(GameObject *owner = nullptr, ComponentTypes type = ComponentTypes::MATERIAL) : Component(owner, type){
        textureId = 0;
        specularId = 0;
        diffuseTurn = true;
        shininess = 0.0f;
    };

    void OnEditor() override
    {
        if (ImGui::CollapsingHeader("Texture"))
        {

            ImGui::Text("Diffuse");
            if (diffuseTurn){
                ImGui::Text("Selected");
            }
            
            ImVec2 uvMin = ImVec2(0.0f, 0.0f);                 // Top-left
            ImVec2 uvMax = ImVec2(1.0f, 1.0f);                 // Lower-right
            ImVec4 tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
            ImVec4 borderCol = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
            ImVec2 sizeImageDisplay = ImVec2(100.0f, 100.0f);
            ImGui::Image((ImTextureID)textureId, sizeImageDisplay, uvMin, uvMax, tintCol, borderCol);

            ImGui::Text("Specular");
            if (!diffuseTurn){
                ImGui::Text("Selected");
            }
            ImGui::Image((ImTextureID)specularId, sizeImageDisplay, uvMin, uvMax, tintCol, borderCol);
        }

        if (ImGui::CollapsingHeader("Options"))
        {
            ImGui::DragFloat("Shininess", &shininess, 1.0f, 0.0f, 255.0f);
        }
    }

    void OnSave(Json::Value &owner) override
    {
        Json::Value materialJson;
        materialJson["type"] = static_cast<int>(ComponentTypes::MATERIAL);
        materialJson["diffuseTexturePath"] = Json::stringValue;
        materialJson["diffuseDirectoryPath"] = Json::stringValue;
        materialJson["specularTexturePath"] = Json::stringValue;
        materialJson["specularDirectoryPath"] = Json::stringValue;
        materialJson["diffuseTurn"] = diffuseTurn;
        materialJson["shininess"] = shininess;

        ResourcesManager resourceManager = App->GetResourcesManager();

        // diffuse material
        std::unordered_map<unsigned int, Texture>::const_iterator found = resourceManager.texturesLoadedInt.find(textureId);
        // found in hash
        if (found != resourceManager.texturesLoadedInt.end())
        {
            materialJson["diffuseTexturePath"] = found->second.path;
            materialJson["diffuseDirectoryPath"] = found->second.directoryPath;
        }

        // specular material
        found = resourceManager.texturesLoadedInt.find(specularId);
        // found in hash
        if (found != resourceManager.texturesLoadedInt.end())
        {
            materialJson["specularTexturePath"] = found->second.path;
            materialJson["specularDirectoryPath"] = found->second.directoryPath;
        }
        
        owner["components"].append(materialJson);
    }

    void OnLoad(const Json::Value &componentJson) override
    {
        textureId = TextureImporter::TextureLoader::GetTextureIdByPath(
            componentJson["diffuseTexturePath"].asString(),
            componentJson["diffuseDirectoryPath"].asString());

        specularId = TextureImporter::TextureLoader::GetTextureIdByPath(
            componentJson["specularTexturePath"].asString(),
            componentJson["specularDirectoryPath"].asString());
        
        diffuseTurn =  componentJson["diffuseTurn"].asBool();
        shininess = componentJson["shininess"].asFloat();

    }

    void setTextureId(const unsigned int textureId)
    {
        this->textureId = textureId;
    }

    void SetSpecularId(const unsigned int specularId)
    {
        this->specularId = specularId;
    }

    void SetTexture(const unsigned int texture)
    {
        if (diffuseTurn)
        {
            textureId = texture;
            diffuseTurn = false;
            return;
        }
        specularId = texture;
        diffuseTurn = true;
    }

    unsigned int GetTextureId()
    {
        return textureId;
    }

    unsigned int GetSpecularId()
    {
        return specularId;
    }
    float GetShininess()
    {
        return shininess;
    }

private:
    unsigned int textureId;
    unsigned int specularId;
    bool diffuseTurn;
    float shininess;
};
