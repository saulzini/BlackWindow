#pragma once

#include <string>

class Texture2D
{
public:
	Texture2D() { imageID = 0; };
	~Texture2D();

	bool loadTexture(const std::string &filename, bool generateMipMaps = true);

	//void bind(GLunit textUnit = 0);

private:

	unsigned int imageID;

};

