#include "Mesh.h"
#include "GL/glew.h"
#include "Vertex.h"
#include "Texture.h"
#include "Core/Importers/Texture/TextureLoader.h"
using namespace std;
Mesh::Mesh(
    const std::vector<Vertex> &mVertices,
    const std::vector<unsigned int> &mIndices,
    const unsigned int textureId)
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

    parent["texturePath"] = Json::stringValue;
    parent["directoryPath"] = Json::stringValue;

    ResourcesManager resourceManager = App->GetResourcesManager();
    std::unordered_map<unsigned int, Texture>::const_iterator found = resourceManager.texturesLoadedInt.find(textureId);
    // found in hash
    if (found != resourceManager.texturesLoadedInt.end())
    {
        parent["texturePath"] = found->second.path;
        parent["directoryPath"] = found->second.directoryPath;
    }
}

void Mesh::LoadFromJson(const Json::Value &component)
{
    Clear();

    // loading vertices
    for (unsigned int i = 0; i < component["vertices"].size(); i++)
    {
        Vertex vertex;
        vertex.LoadFromJson(component["vertices"][i]);
        vertices.push_back(vertex);
    }

    // loading indices
    for (unsigned int i = 0; i < component["indices"].size(); i++)
    {
        indices.push_back(component["indices"][i].asUInt());
    }

    // loading texture
    textureId = TextureImporter::TextureLoader::GetTextureIdByPath(component["texturePath"].asCString(), component["directoryPath"].asCString());

    SetupMesh();
}

// void Mesh::Load(Json::Value &root)
// {

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
    textureId = 0;
}