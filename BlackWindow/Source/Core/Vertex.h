#pragma once

#include "Math/float3.h"
#include "Math/float2.h"
#include "json/json.h"
class Vertex
{
public:
    float3 Position;
    float3 Normal;
    float2 TexCoords;

    float3 GetPosition() const {
        return Position;
    }

    void Save(Json::Value &component)
    {
        Json::Value vertex;
        Json::Value positionJson = Json::arrayValue;
        Json::Value normalJson = Json::arrayValue;
        Json::Value texcoordsJson = Json::arrayValue;

        positionJson.append(Position.x);
        positionJson.append(Position.y);
        positionJson.append(Position.z);

        normalJson.append(Normal.x);
        normalJson.append(Normal.y);
        normalJson.append(Normal.z);

        texcoordsJson.append(TexCoords.x);
        texcoordsJson.append(TexCoords.y);

        vertex["position"] = positionJson;
        vertex["normal"] = normalJson;
        vertex["texcoords"] = texcoordsJson;

        component["vertices"].append(vertex);
    }

    void LoadFromJson(const Json::Value &vertexJson)
    {
        Position = float3(
            vertexJson["position"][0].asFloat(),
            vertexJson["position"][1].asFloat(),
            vertexJson["position"][2].asFloat());
        
        Normal = float3(
            vertexJson["normal"][0].asFloat(),
            vertexJson["normal"][1].asFloat(),
            vertexJson["normal"][2].asFloat());

        TexCoords = float2(
            vertexJson["texcoords"][0].asFloat(),
            vertexJson["texcoords"][1].asFloat());
    }
};
