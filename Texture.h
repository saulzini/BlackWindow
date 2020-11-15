#pragma once
#include <string>
#include "IL/ilu.h"

class Texture
{
	public:
		Texture() {
			id = 0;
		};
		~Texture();
		ILuint id;
		std::string type;
		std::string path;

	private:

};
