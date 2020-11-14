#ifndef __MODULEPROGRAM_H__
#define __MODULEPROGRAM_H__
#include "Module.h"


class ModuleProgram :
    public Module

{
public:
    bool Init();
    update_status PreUpdate(float deltaTime);
    update_status Update(float deltaTime);
    update_status PostUpdate(float deltaTime);
    bool CleanUp();

    char* LoadShaderSource(const char* shaderFileName);

    unsigned CompileShader(unsigned type, const char* source);

    unsigned CreateProgram(unsigned vtxShader, unsigned frgShader);

    unsigned CreateProgramFromSource(const char* vtxShader, const char* frgShader);
    
};

#endif // __MODULEPROGRAM_H__