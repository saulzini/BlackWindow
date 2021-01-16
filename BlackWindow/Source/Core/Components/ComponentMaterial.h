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
    ComponentMaterial(GameObject *owner = nullptr, ComponentTypes type = ComponentTypes::MATERIAL) : Component(owner, type)
    {
        textureId = 0;
        specularId = 0;
        diffuseTurn = true;
        shininess = 0.0f;
    };

    void OnEditor() override;
    void OnSave(Json::Value &owner) override;
    void OnLoad(const Json::Value &componentJson) override;

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
