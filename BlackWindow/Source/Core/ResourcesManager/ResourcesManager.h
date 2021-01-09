#pragma once

#include "Core/Texture.h"
#include <unordered_map>
class ResourcesManager
{
public:
    std::unordered_map<std::string, Texture> texturesLoaded;
    std::unordered_map<unsigned int, Texture> texturesLoadedInt;
};
