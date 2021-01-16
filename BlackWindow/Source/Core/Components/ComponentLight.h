#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include <Math/float3.h>
#include "imgui.h"
#include "debugdraw.h"
#include "Application.h"
#include "Math/Quat.h"
#include "Core/Components/ComponentTransform.h"
//#include "Core/GameObject/GameObject.h"

class ComponentLight : public Component
{
public:
    ComponentLight(GameObject *owner = nullptr, ComponentTypes type = ComponentTypes::LIGHT) : Component(owner, type)
    {
        componentTransform = owner->GetTransformComponent();
        direction = float3(0.0f,0.0f,0.0f);
        rotation = float3(0.0f,0.0f,0.0f);
        specular = float3(0.0f,0.0f,0.0f);
        diffuse = float3(0.0f,0.0f,0.0f);
        ambient = float3(0.0f,0.0f,0.0f);
        azimuth = 0.0f;
        polar = 0.0f;

    };

    void OnSave(Json::Value &owner) override
    {
        Json::Value lightJson;
        lightJson["type"] = static_cast<int>(ComponentTypes::LIGHT);

        std::vector<float3> savingVector{
            direction,
            rotation,
            specular,
            diffuse,
            ambient};

        // saving in order
        lightJson["drsdaMatrix"] = Json::arrayValue;
        for (std::vector<float3>::iterator it = savingVector.begin(); it != savingVector.end(); it++)
        {
            Json::Value valueJson = Json::arrayValue;
            valueJson.append((*it).x);
            valueJson.append((*it).y);
            valueJson.append((*it).z);
            lightJson["drsdaMatrix"].append(valueJson);
        }
        lightJson["azimuth"] = azimuth;
        lightJson["polar"] = polar;

        owner["components"].append(lightJson);
    }

    void OnLoad(const Json::Value &componentJson) override
    {
        for (unsigned int i = 0; i < 5; i++)
        {
            float3 newVec(
                componentJson["drsdaMatrix"][i][0].asFloat(),
                componentJson["drsdaMatrix"][i][1].asFloat(),
                componentJson["drsdaMatrix"][i][2].asFloat());

            switch (i)
            {
            case 0:
                SetDirection(newVec);
                break;
            case 1:
                SetRotation(newVec);
                break;
            case 2:
                SetSpecular(newVec);
                break;
            case 3:
                SetDiffuse(newVec);
                break;
            case 4:
                SetAmbient(newVec);
                break;
            }
        }

        azimuth = componentJson["azimuth"].asFloat();
        polar = componentJson["polar"].asFloat();
    }
    void Update() override
    {

        if (componentTransform == nullptr)
        {
            return;
        }
        float3 arrowFrom = componentTransform->GetPosition();
        float3 arrowTo = arrowFrom + float3(1.0f, 1.0f, 0.0f);
        float radius = 0.5f;
        float3 offset;
        dd::sphere(arrowFrom, dd::colors::Yellow, 1.f, 1.0f);
    }

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

    void OnEditor() override
    {
        if (ImGui::CollapsingHeader("Light"))
        {
            float3 dir = GetDirection();
            float3 rot = GetRotation();
            float3 spec = GetSpecular();
            float3 diff = GetDiffuse();
            float3 amb = GetAmbient();
            float azi = GetAzimuth();
            float pol = GetPolar();

            ImGui::InputFloat3("Direction", &dir[0]);
            ImGui::InputFloat3("Rotation", &rot[0]);
            ImGui::InputFloat3("Specular", &spec[0]);
            ImGui::InputFloat3("Diffuse", &diff[0]);
            ImGui::InputFloat3("Ambient", &amb[0]);
            ImGui::InputFloat("Azimuth", &azi);
            ImGui::InputFloat("Polar", &pol);

            SetDirection(dir);
            SetRotation(rot);
            SetSpecular(spec);
            SetDiffuse(diff);
            SetAmbient(amb);
            SetAzimuth(azi);
            SetPolar(pol);
        }
    }

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

#pragma once
