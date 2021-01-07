#pragma once
#include <string>
#include <vector>
#include <assimp/scene.h>
#include "Texture.h"
#include "Vertex.h"
#include "json/json.h"
class Mesh {
public:
    Mesh() {
        VAO = 0;
        VBO = 0;
        EBO = 0;
    }
    Mesh(const std::vector<Vertex>& mVertices,const std::vector<unsigned int>& mIndices,const std::vector<Texture>& mTextures);

public:
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    void Draw(const unsigned int shader);
    void ChangeTextures(const std::vector<Texture>& textures);

    void Save(Json::Value& parent){
        parent["vertices"] = Json::arrayValue;

        for (std::vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); ++it)
        {
            (*it).Save(parent);
        }
        
        parent["indices"] = Json::arrayValue;
        for (unsigned int i = 0; i != indices.size(); ++i){
            Json::UInt index = indices[i];
            parent["indices"].append( index );
        }

    }

    std::vector<Vertex> GetVertices() const {
        return vertices;
    }

    std::vector<unsigned int> GetIndices() const {
        return indices;
    }

    void Clear(){
       vertices.clear();      
       indices.clear();      
       textures.clear();      
    }

private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void SetupMesh();

};
