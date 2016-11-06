#pragma once
#include <GL\glew.h>
#include <string>
#include <cstddef>
#include "FractalVertex.h"
#include "FractalGLTexture.h"
#include "FractalResourceManager.h"


namespace Fractal
{
	class Sprite2D
	{
	public:
		Sprite2D(); // 클래스 시작시 호출되는 함수 | Function called when the class start
		~Sprite2D(); // 클래스 종료시 호출되는 함수 | Function called when the class end

		void Init(float x, float y, float width, float height, std::string texturePath); // vbo 버퍼 만들기 및 스프라이트 위치와 크기 설정 (x 좌표 위치, y 좌표 위치, 도형 넖이, 도형 높이) | Make vbo buffer and set the sprite position and scale

		void Draw(); // 스프라이트 그리는 함수 | Draw a sprite

	private:
		float _x; // 스프라이트 x 좌표 위치 | Sprite X coordinate position
		float _y; // 스프라이트 y 좌표 위치 | Sprite Y coordinate position
		float _width; // 스프라이트 넓이 | Sprite width
		float _height; // 스프라이트 높이 | Sprite height

		GLuint vboID_2D; // unsignd int 형태의 32bit vbo 버퍼 (2D 전용) | VBO buffer for 2D object

		GLTexture _texture; // 텍스쳐 정보 | Texture ID
	};
}
