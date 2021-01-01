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
        Json::Value currentJson;
        currentJson["vertices"] = Json::arrayValue;

        for (std::vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); ++it)
        {
            (*it).Save(currentJson);
        }
        
        currentJson["indices"] = Json::arrayValue;
        for (unsigned int i = 0; i != indices.size(); ++i){
            Json::UInt index = indices[i];
            currentJson["indices"].append( index );
        }

        parent.append(currentJson);
    }

private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void SetupMesh();

};
