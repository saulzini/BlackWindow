#include "Core/Components/ComponentMeshRenderer.h"
#include "Core/GameObject/GameObject.h"

void ComponentMeshRenderer::Setup()
{
    if (owner == nullptr)
    {
        return;
    }

    ComponentMesh *componentMesh = owner->GetMeshComponent();
    if (componentMesh == nullptr)
    {
        return;
    }

    std::vector<Vertex> vertices = componentMesh->GetVertices();
    std::vector<unsigned int> indices = componentMesh->GetIndices();
    indicesSize = indices.size();
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

void ComponentMeshRenderer::Update()
{
    ComponentMaterial *componentMaterial = owner->GetMaterialComponent();
    if (componentMaterial == nullptr)
    {
        return;
    }

    glUseProgram(owner->GetProgram());
    GLint N = glGetUniformLocation(owner->GetProgram(), "material.shininess");
    glUniform1f(N, componentMaterial->GetShininess());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, componentMaterial->GetTextureId());
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, componentMaterial->GetSpecularId());
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(1);
}

void ComponentMeshRenderer::OnSave(Json::Value& owner) 
{
    Json::Value componentJson;
    componentJson["type"] = static_cast<int>(ComponentTypes::MESHRENDERER);
    owner["components"].append(componentJson);
}


void ComponentMeshRenderer::OnEditor()
{

    if (ImGui::CollapsingHeader("Mesh renderer"))
    {
        ImGui::Text("Mesh indices sizes to render %d", indicesSize);

    }
}