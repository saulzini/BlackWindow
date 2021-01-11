#pragma once
#include <string>
#include <vector>
#include <assimp/scene.h>
#include "Texture.h"
#include "Vertex.h"
#include "json/json.h"
#include "ResourcesManager/ResourcesManager.h"
#include "Application.h"
class Mesh
{
public:
    Mesh()
    {
        vao = 0;
        vbo = 0;
        ebo = 0;
    }
    Mesh(const std::vector<Vertex> &mVertices, const std::vector<unsigned int> &mIndices, unsigned int textureId);

public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int textureId;
    void Draw(const unsigned int shader);

    void ChangeTextures(unsigned int textureId);

    void Save(Json::Value &parent);
    
    void SetIndices(const std::vector<unsigned int> &indices){
        this->indices = indices; 
    }

    void LoadFromJson(const Json::Value &component);
    

    std::vector<Vertex> GetVertices() const
    {
        return vertices;
    }

    std::vector<unsigned int> GetIndices() const
    {
        return indices;
    }

    void Clear();

    void Mesh::Load(Json::Value &root);


private:
    //  render data
    unsigned int vao, vbo, ebo;

    void SetupMesh();
};
