#pragma once

#include "Core/Texture.h"
#include <unordered_map>

class ResourcesManager
{
public:
    static std::unordered_map<std::string, Texture> texturesLoaded;
    static std::unordered_map<unsigned int, Texture> texturesLoadedInt;
};
