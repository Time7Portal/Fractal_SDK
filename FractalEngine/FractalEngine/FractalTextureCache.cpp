#include "FractalTextureCache.h"


Fractal::TextureCache::TextureCache()
{
}


Fractal::TextureCache::~TextureCache()
{
}

Fractal::GLTexture Fractal::TextureCache::GetTexture(std::string texturePath)
{
	// �ؽ��� �����Ͱ� �̹� ĳ�̵Ǿ����� ã�� | Try to find texture file in the map cache
	auto mit = textureMap.find(texturePath); // auto = std::map<std::string, GLTexture>::iterator

	// ã�� ���� ��� | If it's not in the map
	if (mit == textureMap.end())
	{
		// �ؽ��� �ҷ����� | Load the texture
		GLTexture newTexture = ImageLoader::LoadPNG(texturePath);

		// ĳ���ϱ� | Insert it into the map
		std::cout << "[Console] Caching the new texture..." << std::endl;
		textureMap.insert(make_pair(texturePath, newTexture));

		return newTexture;
	}
	else
	{
		// ã�� ��� ĳ�̵� �ؽ��� ��ȯ | Return the cached texture if that was find.
		std::cout << "[Console] Loading the cached texture..." << std::endl;

		return mit->second;
	}
}