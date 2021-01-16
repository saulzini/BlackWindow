#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include <Math/float3.h>
#include "imgui.h"
#include "debugdraw.h"
#include "Application.h"
#include "Math/Quat.h"
#include "Core/Components/ComponentTransform.h"
#include "Core/GameObject/GameObject.h"
class ComponentLight : public Component
{
public:
    ComponentLight(GameObject *owner = nullptr, ComponentTypes type = ComponentTypes::LIGHT) : Component(owner, type)
    {
        componentTransform = owner->GetTransformComponent();
        direction = float3(0.0f, 0.0f, 0.0f);
        rotation = float3(0.0f, 0.0f, 0.0f);
        specular = float3(0.0f, 0.0f, 0.0f);
        diffuse = float3(0.0f, 0.0f, 0.0f);
        ambient = float3(0.0f, 0.0f, 0.0f);
        azimuth = 0.0f;
        polar = 0.0f;
    };

    void OnSave(Json::Value &owner) override;

    void OnLoad(const Json::Value &componentJson) override;

    void Update() override;

    float3 GetDirection() const
    {
        return direction;
    }

    void SetDirection(float3 direction)
    {
        if (!this->direction.Equals(direction))
        {
            this->direction = direction;
        }
    }

    float3 GetSpecular() const
    {
        return specular;
    }

    void SetSpecular(float3 specular)
    {
        if (!this->specular.Equals(specular))
        {
            this->specular = specular;
        }
    }

    float3 GetRotation() const
    {
        return rotation;
    }

    void SetRotation(float3 rotation)
    {
        if (!this->rotation.Equals(rotation))
        {
            this->rotation = rotation;
        }
    }

    float3 GetDiffuse() const
    {
        return diffuse;
    }

    void SetDiffuse(float3 diffuse)
    {
        if (!this->diffuse.Equals(diffuse))
        {
            this->diffuse = diffuse;
        }
    }

    float3 GetAmbient() const
    {
        return ambient;
    }

    void SetAmbient(float3 ambient)
    {
        if (!this->ambient.Equals(ambient))
        {
            this->ambient = ambient;
        }
    }

    float GetAzimuth() const
    {
        return azimuth;
    }

    void SetAzimuth(float azimuth)
    {
        if (this->azimuth != azimuth)
        {
            this->azimuth = azimuth;
        }
    }

    float GetPolar() const
    {
        return polar;
    }

    void SetPolar(float polar)
    {
        if (this->polar != polar)
        {
            this->polar = polar;
        }
    }

    void OnEditor() override;

protected:
    ComponentTransform *componentTransform;
    float azimuth;
    float polar;
    float3 direction;
    float3 rotation;
    float3 specular;
    float3 diffuse;
    float3 ambient;
};
