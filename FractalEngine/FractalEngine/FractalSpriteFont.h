#pragma once
#include <SDL/SDL.h>
#include <TTF/SDL_ttf.h>
#include <GLM\glm.hpp>
#include <map>
#include <vector>
#include "FractalDebug.h"
#include "FractalVertex.h"
#include "FractalSpriteBatch.h"
#define DEFAULT_TEXTURE_RES 256


namespace Fractal
{
	enum class TextAlign // �ؽ�Ʈ ���� ��� ���� | For text justification
	{
		LEFT, MIDDLE, RIGHT
	};


	class SpriteFont // �����ڵ� ����� �ٱ��� ����� ���� �������� �̸� ������ ����
	{
	public:
		SpriteFont(); // Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start
		~SpriteFont(); // Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end

		void Init(const std::string &filePath, int resolutionSize = DEFAULT_TEXTURE_RES); // SDL_TTF �� �ʱ�ȭ �ϴ� �Լ� (���� ���, ��Ʈ ǰ��) | Initialize a SDL_TTF (File path, Font resolution)

		void Destroy(); // SDL_TTF �� ���� �ϴ� �Լ� | Destroy the SDL_TTF

		void TextWrite(std::string newString); // ����� ������ �ۼ��ϴ� �Լ� (�ۼ��� ����) | Write text content to be output (Content string)

		void TextShow(SpriteBatch& newBatch, glm::vec2 newPosition, glm::vec2 newScale, Vertex::ColorRGBA8 newColor); // �ۼ��� ������ ����ϴ� �Լ� (��ġ, ��ġ, ũ��, ��) | Show text (Batch ID, Position, Scale, Color)

	private:
		SDL_Surface* ttfSurface = nullptr; // SDL �ؽ��� | SDL texture pointer
		std::string oldString = ""; // �ؽ�Ʈ ������ �ٲ������ Ȯ���ϱ� ���� ���� ���ڿ� ���� | Save the old string to check the text is changed
		SDL_Color baseColor; // ��Ʈ �⺻ �� (�Ͼ�) | Font base color
		TTF_Font* ttfFont; // ��Ʈ | True type font
		GLuint ttfTextureId; // ��Ʈ ��������Ʈ �ؽ��� ��ȣ | Font sprite texture ID
	};
}