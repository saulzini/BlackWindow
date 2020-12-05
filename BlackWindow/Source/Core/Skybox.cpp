#include "Skybox.h"
#include "Model.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Application.h"
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "Math/float4x4.h"
#include "ModuleWorld.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "../../Resources/glm/ext/matrix_float4x4.hpp"
Skybox::Skybox()
{

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    programSky = App->world->programSky;
    
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
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);


   
    for (unsigned int i = 0; i < files.size(); i++)
    {
        FileManager fileTexture;

        if (fileTexture.ReadFile(files[i].c_str()))
        {

            success = ilLoadL(IL_TYPE_UNKNOWN, fileTexture.GetFile(), fileTexture.GetSize());
            if (success)
            {
                ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
                glGenTextures(1, &textureID);
                glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, textureID);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    ilGetInteger(IL_IMAGE_BPP),
                    ilGetInteger(IL_IMAGE_WIDTH),
                    ilGetInteger(IL_IMAGE_HEIGHT),
                    0,
                    ilGetInteger(IL_IMAGE_FORMAT),
                    GL_UNSIGNED_BYTE,
                    ilGetData());
                glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
                ilDeleteImage(textureID); //release from il since we have it in opengl
            }
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << files[i] << std::endl;

        }

    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

   //lDeleteImage(textureID); //release from il since we have it in opengl
    return textureID;

}



void Skybox::Draw() {



    glUseProgram(programSky);
    float4x4 proj = App->camera->GetProjection();
    float4x4 view = App->camera->GetView();
    
    float4x4 identityModel = float4x4::identity;
    glDepthFunc(GL_LEQUAL);

    glUniform1i(glGetUniformLocation(programSky, "skybox"), 0);
    //glUniformMatrix4fv(glGetUniformLocation(programSky, "model"), 1, GL_TRUE, &identityModel[0][0]); //GL_TRUE transpose the matrix
    glUniformMatrix4fv(glGetUniformLocation(programSky, "view"), 1, GL_TRUE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programSky, "proj"), 1, GL_TRUE, &proj[0][0]);


    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthFunc(GL_LESS); // set depth function back to default
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVAO);
      
}





