#include "Core/Components/ComponentMeshRenderer.h"
#include "Core/GameObject/GameObject.h"
#include "ModuleCamera.h"
#include "Application.h"
#include "ModuleScene.h"
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



   //// GLint N = glGetUniformLocation(program, "material.shininess");

    GLint light_pos = glGetUniformLocation(owner->GetProgram(), "light_pos");
    GLint light_ambient = glGetUniformLocation(owner->GetProgram(), "light.ambient");
    GLint light_diffuse = glGetUniformLocation(owner->GetProgram(), "light.diffuse");
    GLint light_specular = glGetUniformLocation(owner->GetProgram(), "light.specular");
    GLint viewPos = glGetUniformLocation(owner->GetProgram(), "viewPos");
    GLint colorAmbient = glGetUniformLocation(owner->GetProgram(), "colorAmbient");

   // //glUniform1f(N, 32);

    float3 lightpos = App->scene->GetLight()->GetTransformComponent()->GetPosition();
    float3 lightambient = { 0.2f, 0.2f, 0.2f };
    float3 lightdiffuse = { 0.5f, 0.5f, 0.5f };
    float3 lightspecular = { 1.0f, 1.0f, 1.0f };
    float3 view_Pos = App->camera->cameraPosition;
    float3 color_Ambient = { 1.0f, 1.0f, 1.0f };

    /*glUniform1i(glGetUniformLocation(program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(program, "material.specular"), 1);*/

    glUniform3f(light_pos, lightpos[0], lightpos[1], lightpos[2]);
    glUniform3f(light_ambient, lightambient[0], lightambient[1], lightambient[2]);
    glUniform3f(light_diffuse, lightdiffuse[0], lightdiffuse[1], lightdiffuse[2]);
    glUniform3f(light_specular, lightspecular[0], lightspecular[1], lightspecular[2]);
    glUniform3f(viewPos, view_Pos[0], view_Pos[1], view_Pos[2]);
    glUniform3f(colorAmbient, color_Ambient[0], color_Ambient[1], color_Ambient[2]);

}

void ComponentMeshRenderer::OnSave(Json::Value &owner)
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