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
		static GLTexture LoadPNG(std::string filePath); // GLTexture 로 이미지 파일을 반환하는 함수 | Loads images into GLTexturs (File path)
	};
}