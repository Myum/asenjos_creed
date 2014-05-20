#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <map>
#include "texture.h"
#include "includes.h"

class TextureManager
{
	std::map< char*, Texture*> textures;

public:
	TextureManager();
	~TextureManager();

	Texture* getTexture( char* );
};

#endif