#include "FractalSpriteFont.h"


/* - 클래스 시작시 호출되는 함수 | Function called when the class start - */
Fractal::SpriteFont::SpriteFont()
{
	baseColor = { 255, 255, 255, 255 };
}


/* - 클래스 종료시 호출되는 함수 | Function called when the class end - */
Fractal::SpriteFont::~SpriteFont()
{
	Destroy();
}


/* - SDL_TTF 를 초기화 하는 함수 (파일 경로, 폰트 품질) | Initialize a SDL_TTF (File path, Font resolution) - */
void Fractal::SpriteFont::Init(const std::string &filePath, int resolutionSize)
{
	// TTF 폰트 초기화
	if (TTF_Init() == -1) // TTF 초기화에 실패한 경우
	{
		Fractal::Debug::GetInstance()->ConsoleError(std::string(TTF_GetError()) + " - Error occurred during initialization of TTF font!");
	}

	// TTF 폰트 생성
	ttfFont = TTF_OpenFont(filePath.c_str(), resolutionSize);
	if (ttfFont == nullptr) // TTF 생성에 실패한 경우
	{
		Fractal::Debug::GetInstance()->ConsoleError(std::string(TTF_GetError()) + " - Error occurred during creation of TTF font!");
	}

	// GPU 에 텍스쳐 생성 요청하고 사용할 텍스쳐 ID 반환받기
	glGenTextures(1, &ttfTextureId);
}


/* - SDL_TTF 를 제거 하는 함수 | Destroy the SDL_TTF - */
void Fractal::SpriteFont::Destroy()
{
	glDeleteTextures(GL_TEXTURE_2D, &ttfTextureId); // GL 텍스쳐 제거
	SDL_FreeSurface(ttfSurface); // SDL 텍스쳐 제거
	TTF_CloseFont(ttfFont); // 폰트 제거
	TTF_Quit(); // TTF 제거
}


/* - 출력할 내용을 작성하는 함수 (작성할 내용) | Write text content to be output (Content string) - */
void Fractal::SpriteFont::TextWrite(std::string newString)
{
	// 텍스트 내용이 이전과 비교하여 바뀐 경우 출력할 텍스쳐 다시 작성
	if (newString != oldString)
	{
		// 멀티바이트 문자열을 유니코드로 변환
		wchar_t* unicodeText = new wchar_t[newString.length() + 1]; // 유니코드 저장용 변수 지정
		size_t convertedChars = 0; // 변환된 문자 수 카운터
		mbstowcs_s(&convertedChars, unicodeText, newString.length() + 1, newString.c_str(), newString.length()); // MBCS(Multi Byte Character System) -> WBCS(Wide Byte Character System)

		// SDL 텍스쳐로 렌더
		ttfSurface = TTF_RenderUNICODE_Blended(ttfFont, (uint16_t *)unicodeText, baseColor);

		if (ttfSurface == nullptr) // SDL 텍스쳐 생성에 실패한 경우
		{
			Fractal::Debug::GetInstance()->ConsoleError(std::string(TTF_GetError()) + " - Error occurred during creation of TTF texture!");
		}

		// 작성할 텍스쳐 할당받기
		glBindTexture(GL_TEXTURE_2D, ttfTextureId);

		// 밉맵 필터 없이 텍스쳐에 그리기
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ttfSurface->w, ttfSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ttfSurface->pixels);

		// 작성이 끝나면 할당받은 텍스쳐 해제
		glBindTexture(GL_TEXTURE_2D, 0);

		// 텍스트 내용이 변하는지 비교하기 위해 저장
		oldString = newString;
	}
}


/* - 작성된 내용을 출력하는 함수 (배치, 위치, 크기, 색) | Show text (Batch ID, Position, Scale, Color) - */
void Fractal::SpriteFont::TextShow(SpriteBatch& newBatch, glm::vec2 newPosition, glm::vec2 newScale, Vertex::ColorRGBA8 newColor)
{
	glm::vec4 posAndSize = glm::vec4(newPosition.x, newPosition.y, newScale.x, newScale.y); // 위치 와 크기를 vec4로 전환
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f); // UV 사각형 (UV 좌표)

	newBatch.Draw(posAndSize, uv, ttfTextureId, 0.0f, newColor); // 배치 렌더에 포함
}
