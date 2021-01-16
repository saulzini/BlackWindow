#include "Core/Components/ComponentMaterial.h"
void ComponentMaterial::OnEditor()
{
    if (ImGui::CollapsingHeader("Texture"))
    {

        ImGui::Text("Diffuse");
        if (diffuseTurn)
        {
            ImGui::Text("Selected");
        }

        ImVec2 uvMin = ImVec2(0.0f, 0.0f);                 // Top-left
        ImVec2 uvMax = ImVec2(1.0f, 1.0f);                 // Lower-right
        ImVec4 tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
        ImVec4 borderCol = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
        ImVec2 sizeImageDisplay = ImVec2(100.0f, 100.0f);
        ImGui::Image((ImTextureID)textureId, sizeImageDisplay, uvMin, uvMax, tintCol, borderCol);

        ImGui::Text("Specular");
        if (!diffuseTurn)
        {
            ImGui::Text("Selected");
        }
        ImGui::Image((ImTextureID)specularId, sizeImageDisplay, uvMin, uvMax, tintCol, borderCol);
    }

    if (ImGui::CollapsingHeader("Options"))
    {
        ImGui::DragFloat("Shininess", &shininess, 1.0f, 0.0f, 255.0f);
    }
}

void ComponentMaterial::OnSave(Json::Value &owner)
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

void ComponentMaterial::OnLoad(const Json::Value &componentJson)
{
    std::string texturePath = componentJson["diffuseTexturePath"].asString();
    if (texturePath != "")
    {
        textureId = TextureImporter::TextureLoader::GetTextureIdByPath(
            texturePath,
            componentJson["diffuseDirectoryPath"].asString());
    }

    texturePath = componentJson["specularTexturePath"].asString();
    if (texturePath != "")
    {
        specularId = TextureImporter::TextureLoader::GetTextureIdByPath(
            texturePath,
            componentJson["specularDirectoryPath"].asString());
    }
    diffuseTurn = componentJson["diffuseTurn"].asBool();
    shininess = componentJson["shininess"].asFloat();
}
