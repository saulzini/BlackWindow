#include "Core/Components/ComponentTransform.h"


void ComponentTransform::OnEditor() 
{
    if (ImGui::CollapsingHeader("Transformation"))
    {
        float3 auxPosition = GetPosition();
        float3 auxScale = GetScale();
        float3 auxRotation = GetRotation();
        ImGui::InputFloat3("Position", &auxPosition[0]);
        ImGui::InputFloat3("Scale", &auxScale[0]);
        ImGui::InputFloat3("Rotation", &auxRotation[0]);
        SetPosition(auxPosition);
        SetScale(auxScale);
        SetRotation(auxRotation);
    }
}

void ComponentTransform::OnSave(Json::Value& owner) 
{
    Json::Value transformJson;
    transformJson["type"] = static_cast<int>(ComponentTypes::TRANSFORM);

    Json::Value positionJson = Json::arrayValue;
    Json::Value scaleJson = Json::arrayValue;
    Json::Value rotationJson = Json::arrayValue;

    positionJson.append( position.x );
    positionJson.append( position.y );
    positionJson.append( position.z );

    scaleJson.append( scale.x );
    scaleJson.append( scale.y );
    scaleJson.append( scale.z );

    rotationJson.append( rotation.x );
    rotationJson.append( rotation.y );
    rotationJson.append( rotation.z );

    transformJson["position"] = positionJson;
    transformJson["scale"] = scaleJson;
    transformJson["rotation"] = rotationJson;

    owner["components"].append(transformJson);
}

void ComponentTransform::OnLoad(const Json::Value& componentJson) 
{
    SetPosition( float3( componentJson["position"][0].asFloat() , componentJson["position"][1].asFloat() , componentJson["position"][2].asFloat() ) );
    SetScale( float3( componentJson["scale"][0].asFloat() , componentJson["scale"][1].asFloat() , componentJson["scale"][2].asFloat() ) );
    SetRotation( float3( componentJson["rotation"][0].asFloat() , componentJson["rotation"][1].asFloat() , componentJson["rotation"][2].asFloat() ) );
}
