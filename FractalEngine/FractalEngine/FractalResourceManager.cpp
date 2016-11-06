#include "FractalResourceManager.h"


Fractal::TextureCache Fractal::ResourceManager::m_TextureCache; // ���� �ɹ� ���� ����


/* - �ؽ��ĸ� �о���̰� ĳ���ϴ� ���� �Լ� | Static function for read texture and texture caching - */
Fractal::GLTexture Fractal::ResourceManager::GetTexture(std::string texturePath)
{
	return m_TextureCache.GetTexture(texturePath);
}