#include "ModuleProgram.h"
#include <stdio.h>
#include <stdlib.h>  
#include <GL\glew.h>
#include "Globals.h"

bool ModuleProgram::Init()
{
    return true;
}

update_status ModuleProgram::PreUpdate()
{
    return UPDATE_CONTINUE;
}

update_status ModuleProgram::Update()
{
    return UPDATE_CONTINUE;
}

update_status ModuleProgram::PostUpdate()
{
    return UPDATE_CONTINUE;
}

bool ModuleProgram::CleanUp()
{
    return true;
}

char* ModuleProgram::LoadShaderSource(const char* shader_file_name)
{
    char* data = nullptr;
    FILE* file = nullptr;
    fopen_s(&file, shader_file_name, "rb");
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
    unsigned shader_id = glCreateShader(type);
    glShaderSource(shader_id, 1, &source, 0);
    glCompileShader(shader_id);
    int res = GL_FALSE;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
    if (res == GL_FALSE)
    {
        int len = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
        if (len > 0)
        {
            int written = 0;
            char* info = (char*)malloc(len);
            glGetShaderInfoLog(shader_id, len, &written, info);
            LOG("Log Info: %s", info);
            free(info);
        }
    }
    return shader_id;
}

unsigned ModuleProgram::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
{
    unsigned program_id = glCreateProgram();
    glAttachShader(program_id, vtx_shader);
    glAttachShader(program_id, frg_shader);
    glLinkProgram(program_id);
    int res;
    glGetProgramiv(program_id, GL_LINK_STATUS, &res);
    if (res == GL_FALSE)
    {
        int len = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
        if (len > 0)
        {
            int written = 0;
            char* info = (char*)malloc(len);
            glGetProgramInfoLog(program_id, len, &written, info);
            LOG("Program Log Info: %s", info);
            free(info);
        }
    }
    glDeleteShader(vtx_shader);
    glDeleteShader(frg_shader);
    return program_id;
}

unsigned ModuleProgram::CreateProgramFromSource(const char* vtx_shader, const char* frg_shader)
{
    char* vtx_source = LoadShaderSource(vtx_shader);
    char* frg_source = LoadShaderSource(frg_shader);
    
    if (vtx_shader == nullptr || frg_source == nullptr) {
        return 0;
    }

    unsigned vtx_compile = CompileShader(GL_VERTEX_SHADER, vtx_source);
    unsigned frg_compile = CompileShader(GL_FRAGMENT_SHADER, frg_source);
    if (vtx_shader == 0 || frg_shader == 0) {
        return 0;
    }

    return CreateProgram(vtx_compile, frg_compile);
}
