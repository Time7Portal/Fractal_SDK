#include "FractalTextureCache.h"


Fractal::TextureCache::TextureCache()
{
}


Fractal::TextureCache::~TextureCache()
{
}

Fractal::GLTexture Fractal::TextureCache::GetTexture(std::string texturePath)
{
	// 텍스쳐 데이터가 이미 캐싱되었는지 찾기 | Try to find texture file in the map cache
	auto mit = textureMap.find(texturePath); // auto = std::map<std::string, GLTexture>::iterator

	// 찾지 못한 경우 | If it's not in the map
	if (mit == textureMap.end())
	{
		// 텍스쳐 불러오기 | Load the texture
		GLTexture newTexture = ImageLoader::LoadPNG(texturePath);

		// 캐싱하기 | Insert it into the map
		std::cout << "[Console] Caching the new texture..." << std::endl;
		textureMap.insert(make_pair(texturePath, newTexture));

		return newTexture;
	}
	else
	{
		// 찾은 경우 캐싱된 텍스쳐 반환 | Return the cached texture if that was find.
		std::cout << "[Console] Loading the cached texture..." << std::endl;

		return mit->second;
	}
}