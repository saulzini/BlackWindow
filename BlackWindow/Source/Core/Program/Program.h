#ifndef __MODULEPROGRAM_H__
#define __MODULEPROGRAM_H__
#include "Module.h"

// TODO::Change to static
class Program 
{
public:
    char* LoadShaderSource(const char* shaderFileName);
    unsigned CompileShader(unsigned type, const char* source);
    unsigned CreateProgram(unsigned vtxShader, unsigned frgShader);
    unsigned CreateProgramFromSource(const char* vtxShader, const char* frgShader);
};

#endif // __MODULEPROGRAM_H__