#pragma once
#include <string>

class Texture
{
public:
	Texture()
	{
		id = 0;
	};
	unsigned int id;
	std::string path;
	std::string directoryPath;
};
