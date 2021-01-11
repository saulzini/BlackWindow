#include "Skybox.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Application.h"
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "Core/Program/Program.h"
#include "Math/float4x4.h"
#include "ModuleScene.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "../../Resources/glm/ext/matrix_float4x4.hpp"
#include <IL/il.h>
#include <Core/FileManager.h>
Skybox::Skybox()
{
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //programSky = App->scene->programSky;
    
   // this->loadCubemap(files);

}

Skybox::~Skybox()
{
    glDeleteBuffers(1, &vbo);
}


unsigned int Skybox::loadCubemap(std::vector<std::string> files)
{
    ILboolean success = false;
    ILuint textureID;

    ilGenImages(1, &textureID);
    // bind it
    ilBindImage(textureID);
    // match image origin to OpenGLs
    ilEnable(IL_ORIGIN_SET);
   // ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

   
    for (unsigned int i = 0; i < files.size(); i++)
    {
        FileManager fileTexture;
        std::string resultPath("");
        resultPath = "";
        resultPath.append(files[i].c_str());

        if (fileTexture.ReadFile(resultPath.c_str()))
        {

            success = ilLoadL(IL_TYPE_UNKNOWN, fileTexture.GetFile(), fileTexture.GetSize());
            if (success)
            {
               // ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
               // glGenTextures(1, &textureID);
               // glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, textureID);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    ilGetInteger(IL_IMAGE_BPP),
                    ilGetInteger(IL_IMAGE_WIDTH),
                    ilGetInteger(IL_IMAGE_HEIGHT),
                    0,
                    ilGetInteger(IL_IMAGE_FORMAT),
                    GL_UNSIGNED_BYTE,
                    ilGetData());
               // glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
                //ilDeleteImage(textureID); //release from il since we have it in opengl
            }
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << files[i] << std::endl;

        }

    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    ilDeleteImage(textureID); //release from il since we have it in opengl
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    //glDeleteTextures(1,textureID); //release from il since we have it in opengl
    return textureID;

}



void Skybox::Draw() {



    glDepthMask(GL_FALSE); // superfluous
    glDepthFunc(GL_LEQUAL);
    glUseProgram(App->scene->programSky);
    float4x4 proj = App->camera->GetProjection();
    float4x4 view = App->camera->GetView();
    // view = glm::mat4(glm::mat3(App->camera->GetView())); // remove translation from the view matrix
     //float4x4 identityModel = float4x4::identity;
    // glDepthFunc(GL_LEQUAL);

    glUniform1i(glGetUniformLocation(App->scene->programSky, "Skybox"), 0);
    //glUniformMatrix4fv(glGetUniformLocation(programSky, "model"), 1, GL_TRUE, &identityModel[0][0]); //GL_TRUE transpose the matrix
    glUniformMatrix4fv(glGetUniformLocation(App->scene->programSky, "view"), 1, GL_TRUE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(App->scene->programSky, "proj"), 1, GL_TRUE, &proj[0][0]);



    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
    glDepthMask(GL_TRUE); // superfluous
    //glDeleteVertexArrays(1, &skyboxVAO);
   // glDeleteBuffers(1, &skyboxVAO);
   /// glDeleteProgram(App->world->programSky);
}





