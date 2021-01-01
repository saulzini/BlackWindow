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
        parent["position"] = Json::arrayValue;
        parent["normal"] = Json::arrayValue;
        parent["texcoords"] = Json::arrayValue;

        parent["position"].append( Position.x );
        parent["position"].append( Position.y );
        parent["position"].append( Position.z );

        parent["normal"].append( Normal.x );
        parent["normal"].append( Normal.y );
        parent["normal"].append( Normal.z );

        parent["texcoords"].append( TexCoords.x );
        parent["texcoords"].append( TexCoords.y );
    }
};

