#pragma once
#include <string>
#include <vector>
#include <assimp/scene.h>
#include "Texture.h"
#include "Vertex.h"

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

public:
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    void Draw(const unsigned int shader);
    void ChangeTextures(std::vector<Texture> textures);

private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void SetupMesh();

};
