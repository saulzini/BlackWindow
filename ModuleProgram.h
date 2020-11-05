#ifndef __MODULEPROGRAM_H__
#define __MODULEPROGRAM_H__
#include "Module.h"


class ModuleProgram :
    public Module

{
public:
    bool Init();
    update_status PreUpdate();
    update_status Update();
    update_status PostUpdate();
    bool CleanUp();

    char* LoadShaderSource(const char* shader_file_name);

    unsigned CompileShader(unsigned type, const char* source);

    unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);

    unsigned CreateProgramFromSource(const char* vtx_shader, const char* frg_shader);
    
};

#endif // __MODULEPROGRAM_H__