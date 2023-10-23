#include "resources_singleton.h"

#include <cassert>

ResourcesSingleton& ResourcesSingleton::GetInstance()
{
	static ResourcesSingleton instance;

	return instance;
}

Texture2D& ResourcesSingleton::GetCellAtlasTexture()
{
	return cellAtlasTexture;
}

ResourcesSingleton::ResourcesSingleton()
{
	assert(GetWindowHandle() && "Cannot load texture data, since window doesn't exist");

	const Image cellAtlasImage = LoadImage("resources/sprite_atlas.png");
	cellAtlasTexture = LoadTextureFromImage(cellAtlasImage);
	UnloadImage(cellAtlasImage);
}

ResourcesSingleton::~ResourcesSingleton()
{
	UnloadTexture(cellAtlasTexture);
}
