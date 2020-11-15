#pragma once
#include <string>
#include <vector>
struct vec3 {
    int x = 0; 
    int y = 0; 
    int z = 0;
};

struct vec2 {
    int x = 0;
    int y = 0;
};

struct Vertex {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
};
class Mesh {
public:
    Mesh() {};
   /* Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        SetupMesh();
    }
 
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);*/
private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void SetupMesh();
};
