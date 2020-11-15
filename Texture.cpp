#include "Texture.h"

Texture::~Texture()
{
	ilBindImage(0);
	ilDeleteImage(id);
}
