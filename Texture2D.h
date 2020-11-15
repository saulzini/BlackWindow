#pragma once

#include <string>
#include "IL/ilu.h"

class Texture2D
{
public:
	Texture2D() {
		imageID = 0;
	};
	~Texture2D();

	ILuint LoadTexture(const std::string &filename, bool generateMipMaps = true);

	//void bind(GLunit textUnit = 0);

private:

	ILuint  imageID;

};

