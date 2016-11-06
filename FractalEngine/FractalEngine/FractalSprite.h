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
		Sprite2D(); // Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start
		~Sprite2D(); // Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end

		void Init(float x, float y, float width, float height, std::string texturePath); // vbo ���� ����� �� ��������Ʈ ��ġ�� ũ�� ���� (x ��ǥ ��ġ, y ��ǥ ��ġ, ���� ����, ���� ����) | Make vbo buffer and set the sprite position and scale

		void Draw(); // ��������Ʈ �׸��� �Լ� | Draw a sprite

	private:
		float _x; // ��������Ʈ x ��ǥ ��ġ | Sprite X coordinate position
		float _y; // ��������Ʈ y ��ǥ ��ġ | Sprite Y coordinate position
		float _width; // ��������Ʈ ���� | Sprite width
		float _height; // ��������Ʈ ���� | Sprite height

		GLuint vboID_2D; // unsignd int ������ 32bit vbo ���� (2D ����) | VBO buffer for 2D object

		GLTexture _texture; // �ؽ��� ���� | Texture ID
	};
}
