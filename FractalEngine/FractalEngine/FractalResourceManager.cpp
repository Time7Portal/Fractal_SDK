#include "FractalResourceManager.h"


Fractal::TextureCache Fractal::ResourceManager::m_TextureCache; // 정적 맴버 변수 노출


/* - 텍스쳐를 읽어들이고 캐싱하는 정적 함수 | Static function for read texture and texture caching - */
Fractal::GLTexture Fractal::ResourceManager::GetTexture(std::string texturePath)
{
	return m_TextureCache.GetTexture(texturePath);
}