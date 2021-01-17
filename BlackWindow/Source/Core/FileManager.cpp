#include "FileManager.h"
#include <string>
#include <Windows.h>
bool FileManager::ReadFile(const char* path) 
{
    std::ifstream file (path, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {

        // Read file
        size = file.tellg();
        memblock = new char [size];
        file.seekg (0, std::ios::beg);
        file.read (memblock, size);
        file.close();
        
        // create copy inside texture file
        std::string fileName = path;
        if (fileName.find_last_of('\\') != std::string::npos) {
            fileName = fileName.substr(fileName.find_last_of('\\'), fileName.size() );

        }
        std::string result = "Assets/Textures/";
        result.append(fileName) ;
        
        CopyFile(path,result.c_str(),0);

        return 1;
    }
    return 0;
}
