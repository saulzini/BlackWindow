#pragma once
#include "Assimp/vector3.h"
#include "Assimp/vector2.h"
class Vertex
{
public:
    aiVector3t<float> Position;
    aiVector3t<float> Normal;
    aiVector2t<float> TexCoords;
};

