#pragma once
#include "FractalDebug.h"
#include "FractalPicoPNG.h"
#include "FractalFileIOManager.h"
#include "FractalGLTexture.h"


namespace Fractal
{
	class ImageLoader
	{
	public:
		static GLTexture LoadPNG(std::string filePath); // GLTexture �� �̹��� ������ ��ȯ�ϴ� �Լ� | Loads images into GLTexturs (File path)
	};
}