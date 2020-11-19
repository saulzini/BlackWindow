#pragma once

#include <string>
#include "IL/ilu.h"
//TODO::CHANGE to a loader 
class Texture2D
{
public:
	Texture2D() {
	};
	~Texture2D();

	bool LoadTexture(const std::string &filename, bool generateMipMaps = true);

private:
};

