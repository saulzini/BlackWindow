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

    void Save(Json::Value& parent){
        Json::Value vertex;
        Json::Value positionJson = Json::arrayValue;
        Json::Value normalJson = Json::arrayValue;
        Json::Value texcoordsJson = Json::arrayValue;

        positionJson.append( Position.x );
        positionJson.append( Position.y );
        positionJson.append( Position.z );

        normalJson.append( Normal.x );
        normalJson.append( Normal.y );
        normalJson.append( Normal.z );

        texcoordsJson.append( TexCoords.x );
        texcoordsJson.append( TexCoords.y );

        vertex["position"] = positionJson;
        vertex["normal"] = normalJson;
        vertex["texcoords"] = texcoordsJson;

        parent["vertices"].append(vertex);

    }
};

