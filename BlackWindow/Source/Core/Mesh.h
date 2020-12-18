#pragma once
#include <string>
#include <vector>
#include <assimp/scene.h>
#include "Texture.h"
#include "Vertex.h"

class Mesh {
public:
    Mesh() {
        VAO = 0;
        VBO = 0;
        EBO = 0;
    }
    Mesh(std::vector<Vertex>& mVertices, std::vector<unsigned int>& mIndices, std::vector<Texture>& mTextures);

public:
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    void Draw(const unsigned int shader);
    void ChangeTextures(const std::vector<Texture>& textures);

private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void SetupMesh();

};
