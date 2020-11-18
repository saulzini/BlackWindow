#pragma once
#include <string>

class Texture
{
	public:
		Texture() {
			id = 0;
		};
		~Texture();
		unsigned int id;
		std::string type;
		std::string path;

	private:

};
