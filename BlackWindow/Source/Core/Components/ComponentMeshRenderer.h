#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include <Math/float3.h>
#include "imgui.h"
#include "Math/Quat.h"
#include <vector>
#include "Core/Texture.h"
#include "GL/glew.h"
#include "Core/Components/ComponentMaterial.h"
#include "Core/Components/ComponentMesh.h"
class ComponentMeshRenderer : public Component
{
public:
    ComponentMeshRenderer(GameObject *owner = nullptr, ComponentTypes type = ComponentTypes::MESHRENDERER) : Component(owner, type){};

    void Setup()
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




    void Update() override
    {
       

        ComponentMaterial* componentMaterial = owner->GetMaterialComponent();
        if (componentMaterial == nullptr)
        {
            return;
        }


        glUseProgram(owner->GetProgram());
        GLint N = glGetUniformLocation(owner->GetProgram(), "material.shininess");
        glUniform1f(N, componentMaterial->GetShininess());


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, componentMaterial->GetTextureId() );
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, componentMaterial->GetSpecularId());
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
        glBindVertexArray(1);
        
    }

private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int ebo = 0;
    size_t indicesSize =0;
};
