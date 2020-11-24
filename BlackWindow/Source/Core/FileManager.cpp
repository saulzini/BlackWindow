#include "FileManager.h"

bool FileManager::ReadFile(const char* path) 
{
    std::ifstream file (path, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char [size];
        file.seekg (0, std::ios::beg);
        file.read (memblock, size);
        file.close();
        return 1;
    }
    return 0;
}
