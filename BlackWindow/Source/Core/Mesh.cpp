#include "Mesh.h"
#include "GL/glew.h"
#include "Vertex.h"
#include "Texture.h"

using namespace std;
Mesh::Mesh(
    const std::vector<Vertex> &mVertices,
    const std::vector<unsigned int> &mIndices,
    unsigned int textureId)
{
    vao = 0;
    vbo = 0;
    ebo = 0;
    this->vertices = mVertices;
    this->indices = mIndices;
    this->textureId = textureId;

    SetupMesh();
}

void Mesh::Draw(const unsigned int shader)
{
    glBindTexture(GL_TEXTURE_2D, textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void Mesh::SetupMesh()
{

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Mesh::ChangeTextures(unsigned int textureId)
{
    this->textureId = textureId;
    SetupMesh();
}

void Mesh::Save(Json::Value &parent)
{
    parent["vertices"] = Json::arrayValue;

    for (std::vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        (*it).Save(parent);
    }

    parent["indices"] = Json::arrayValue;
    for (unsigned int i = 0; i != indices.size(); ++i)
    {
        Json::UInt index = indices[i];
        parent["indices"].append(index);
    }

    parent["texture"] = Json::stringValue;

    ResourcesManager resourceManager = App->GetResourcesManager();
    std::unordered_map<unsigned int, Texture>::const_iterator found = resourceManager.texturesLoadedInt.find(textureId);
    // found in hash
    if (found != resourceManager.texturesLoadedInt.end())
    {
        parent["texture"] = found->second.path;
    }
}



// void Mesh::Load(Json::Value &root)
// {

//     for (unsigned int i=0; i< root["vertices"].size(); i++)
//     {

//     }

//     for (unsigned int i=0; i< root["indices"].size(); i++)
//     {
//         root["indices"].append( root["indices"][i] );
//     }



//     root["texture"] = Json::stringValue;

//     ResourcesManager resourceManager = App->GetResourcesManager();
//     std::unordered_map<unsigned int, Texture>::const_iterator found = resourceManager.texturesLoadedInt.find(textureId);
//     // found in hash
//     if (found != resourceManager.texturesLoadedInt.end())
//     {
//         root["texture"] = found->second.path;
//     }
// }

void Mesh::Clear()
{
    vertices.clear();
    indices.clear();
}