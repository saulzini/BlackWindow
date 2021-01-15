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
            ImGui::Image((ImTextureID)textureId, sizeImageDisplay, uvMin, uvMax, tintCol, borderCol);

            ImGui::Text("Specular");
            ImGui::Image((ImTextureID)specularId, sizeImageDisplay, uvMin, uvMax, tintCol, borderCol);
        }

        if (ImGui::CollapsingHeader("OPTIONS"))
        {
            ImGui::DragFloat("Shininess", &shininess, 1.0f, 0.0f, 255.0f);
        }
    }

    void OnSave(Json::Value &parent) override
    {
        parent["diffuseTexturePath"] = Json::stringValue;
        parent["diffuseDirectoryPath"] = Json::stringValue;
        parent["specularTexturePath"] = Json::stringValue;
        parent["specularDirectoryPath"] = Json::stringValue;

        ResourcesManager resourceManager = App->GetResourcesManager();

        // diffuse material
        std::unordered_map<unsigned int, Texture>::const_iterator found = resourceManager.texturesLoadedInt.find(textureId);
        // found in hash
        if (found != resourceManager.texturesLoadedInt.end())
        {
            parent["diffuseTexturePath"] = found->second.path;
            parent["diffuseDirectoryPath"] = found->second.directoryPath;
        }

        // specular material
        found = resourceManager.texturesLoadedInt.find(specularId);
        // found in hash
        if (found != resourceManager.texturesLoadedInt.end())
        {
            parent["specularTexturePath"] = found->second.path;
            parent["specularDirectoryPath"] = found->second.directoryPath;
        }
    }

    void OnLoad(const Json::Value &componentJson) override
    {
        textureId = TextureImporter::TextureLoader::GetTextureIdByPath(
            componentJson["diffuseTexturePath"].asString(),
            componentJson["diffuseDirectoryPath"].asString());

        specularId = TextureImporter::TextureLoader::GetTextureIdByPath(
            componentJson["specularTexturePath"].asString(),
            componentJson["specularDirectoryPath"].asString());
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
        if (textureId == 0)
        {
            textureId = texture;
        }
        else if (specularId == 0)
        {
            specularId = texture;
        }
        else
        {
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
    float GetShininess()
    {
        return shininess;
    }

private:
    unsigned int textureId;
    unsigned int specularId;
    float shininess = 0;
};
