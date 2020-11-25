#include "ModuleProgram.h"
#include <stdio.h>
#include <stdlib.h>  
#include <GL\glew.h>
#include "Globals.h"
#include <assert.h>  
bool ModuleProgram::Init()
{
    return true;
}

update_status ModuleProgram::PreUpdate(float deltaTime)
{
    return UPDATE_CONTINUE;
}

update_status ModuleProgram::Update(float deltaTime)
{
    return UPDATE_CONTINUE;
}

update_status ModuleProgram::PostUpdate(float deltaTime)
{
    return UPDATE_CONTINUE;
}

bool ModuleProgram::CleanUp()
{
    return true;
}

char* ModuleProgram::LoadShaderSource(const char* shaderFileName)
{
    char* data = nullptr;
    FILE* file = nullptr;
    fopen_s(&file, shaderFileName, "rb");
    if (file)
    {
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        data = (char*)malloc(size + 1);
        fseek(file, 0, SEEK_SET);
        fread(data, 1, size, file);
        data[size] = 0;
        fclose(file);
    }
    return data;
}

unsigned ModuleProgram::CompileShader(unsigned type, const char* source)
{
    unsigned shaderId = glCreateShader(type);
    glShaderSource(shaderId, 1, &source, 0);
    glCompileShader(shaderId);
    int res = GL_FALSE;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &res);
    if (res == GL_FALSE)
    {
        int len = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &len);
        if (len > 0)
        {
            int written = 0;
            char* info = (char*)malloc(len);
            glGetShaderInfoLog(shaderId, len, &written, info);
            // LOG("Log Info: %s", info);
            free(info);
        }
        assert(res != GL_FALSE); // Shader not compiled
    }
    return shaderId;
}

unsigned ModuleProgram::CreateProgram(unsigned vtxShader, unsigned frgShader)
{
    unsigned programId = glCreateProgram();
    glAttachShader(programId, vtxShader);
    glAttachShader(programId, frgShader);
    glLinkProgram(programId);
    int res;
    glGetProgramiv(programId, GL_LINK_STATUS, &res);
    if (res == GL_FALSE)
    {
        int len = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &len);
        if (len > 0)
        {
            int written = 0;
            char* info = (char*)malloc(len);
            glGetProgramInfoLog(programId, len, &written, info);
            // LOG("Program Log Info: %s", info);
            free(info);
        }
    }
    glDeleteShader(vtxShader);
    glDeleteShader(frgShader);
    return programId;
}

unsigned ModuleProgram::CreateProgramFromSource(const char* vtxShader, const char* frgShader)
{
    char* vtxSource = LoadShaderSource(vtxShader);
    char* frgSource = LoadShaderSource(frgShader);
    
    assert(vtxSource != nullptr); // Source not found
    assert(frgSource != nullptr); // Source not found

    unsigned vtxCompile = CompileShader(GL_VERTEX_SHADER, vtxSource);
    unsigned frgCompile = CompileShader(GL_FRAGMENT_SHADER, frgSource);
   

    return CreateProgram(vtxCompile, frgCompile);
}
