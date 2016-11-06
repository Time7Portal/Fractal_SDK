#pragma once
#include <string>
#include "FractalTextureCache.h"


namespace Fractal
{
	class ResourceManager
	{
	public:
		static GLTexture GetTexture(std::string texturePath); // 텍스쳐를 읽어들이고 캐싱하는 정적 함수 | Static function for read texture and texture caching

	private:
		static TextureCache m_TextureCache; // 텍스쳐 캐싱을 위한 정적 맴버 변수 | Static member variable for texture caching
	};
}