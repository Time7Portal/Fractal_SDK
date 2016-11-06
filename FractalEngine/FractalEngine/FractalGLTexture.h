#pragma once
#include <GL/glew.h>


namespace Fractal
{
	struct GLTexture // GLTexture 구조체 정의
	{
		GLuint id;
		int width;
		int height;
	};
}