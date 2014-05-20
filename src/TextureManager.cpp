#include "TextureManager.h"

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{

}

Texture* TextureManager::getTexture(char* textureFile)
{
	//return NULL;
	std::map< char*, Texture* >::iterator it = this->textures.find(textureFile);

	if(it == this->textures.end())
	{
		Texture* texture = new Texture();
		texture->load( textureFile );
		this->textures.insert( std::pair< char* , Texture* >(textureFile, texture) );

		return texture;
	}
	else
	{
		return it->second;
	}
}