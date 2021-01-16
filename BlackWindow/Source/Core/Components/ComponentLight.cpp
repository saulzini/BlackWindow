#include "Core/Components/ComponentLight.h"

void ComponentLight::OnSave(Json::Value &owner)
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

void ComponentLight::OnLoad(const Json::Value &componentJson)
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

void ComponentLight::Update()
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

void ComponentLight::OnEditor()
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
