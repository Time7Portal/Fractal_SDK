#pragma once
#include <iostream>
#include <map>
#include "FractalGLTexture.h"
#include "FractalImageLoader.h"


namespace Fractal
{
	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		GLTexture GetTexture(std::string texturePath);

	private:
		std::map<std::string, GLTexture> textureMap;
	};
}