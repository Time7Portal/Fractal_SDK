#pragma once
#include <string>
#include "FractalTextureCache.h"


namespace Fractal
{
	class ResourceManager
	{
	public:
		static GLTexture GetTexture(std::string texturePath); // �ؽ��ĸ� �о���̰� ĳ���ϴ� ���� �Լ� | Static function for read texture and texture caching

	private:
		static TextureCache m_TextureCache; // �ؽ��� ĳ���� ���� ���� �ɹ� ���� | Static member variable for texture caching
	};
}