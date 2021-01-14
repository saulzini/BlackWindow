#pragma once
#include <vector>
#include "Core/Vertex.h"
class ProcessedMesh
{
public:
    ProcessedMesh(
        const std::vector<Vertex> &vertices,
        const std::vector<unsigned int> &indices,
        const unsigned int textureId,
        const unsigned int specularId)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textureId = textureId;
        this->specularId = specularId;
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int textureId;
    unsigned int specularId;
};
