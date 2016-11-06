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
	enum class TextAlign // 텍스트 정렬 방식 정의 | For text justification
	{
		LEFT, MIDDLE, RIGHT
	};


	class SpriteFont // 유니코드 방식은 다국어 출력을 위해 문자판을 미리 만들지 않음
	{
	public:
		SpriteFont(); // 클래스 시작시 호출되는 함수 | Function called when the class start
		~SpriteFont(); // 클래스 종료시 호출되는 함수 | Function called when the class end

		void Init(const std::string &filePath, int resolutionSize = DEFAULT_TEXTURE_RES); // SDL_TTF 를 초기화 하는 함수 (파일 경로, 폰트 품질) | Initialize a SDL_TTF (File path, Font resolution)

		void Destroy(); // SDL_TTF 를 제거 하는 함수 | Destroy the SDL_TTF

		void TextWrite(std::string newString); // 출력할 내용을 작성하는 함수 (작성할 내용) | Write text content to be output (Content string)

		void TextShow(SpriteBatch& newBatch, glm::vec2 newPosition, glm::vec2 newScale, Vertex::ColorRGBA8 newColor); // 작성된 내용을 출력하는 함수 (배치, 위치, 크기, 색) | Show text (Batch ID, Position, Scale, Color)

	private:
		SDL_Surface* ttfSurface = nullptr; // SDL 텍스쳐 | SDL texture pointer
		std::string oldString = ""; // 텍스트 내용이 바뀌었는지 확인하기 위해 이전 문자열 저장 | Save the old string to check the text is changed
		SDL_Color baseColor; // 폰트 기본 색 (하양) | Font base color
		TTF_Font* ttfFont; // 폰트 | True type font
		GLuint ttfTextureId; // 폰트 스프라이트 텍스쳐 번호 | Font sprite texture ID
	};
}