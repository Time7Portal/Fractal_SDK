#include "FractalSpriteFont.h"


/* - Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start - */
Fractal::SpriteFont::SpriteFont()
{
	baseColor = { 255, 255, 255, 255 };
}


/* - Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end - */
Fractal::SpriteFont::~SpriteFont()
{
	Destroy();
}


/* - SDL_TTF �� �ʱ�ȭ �ϴ� �Լ� (���� ���, ��Ʈ ǰ��) | Initialize a SDL_TTF (File path, Font resolution) - */
void Fractal::SpriteFont::Init(const std::string &filePath, int resolutionSize)
{
	// TTF ��Ʈ �ʱ�ȭ
	if (TTF_Init() == -1) // TTF �ʱ�ȭ�� ������ ���
	{
		Fractal::Debug::GetInstance()->ConsoleError(std::string(TTF_GetError()) + " - Error occurred during initialization of TTF font!");
	}

	// TTF ��Ʈ ����
	ttfFont = TTF_OpenFont(filePath.c_str(), resolutionSize);
	if (ttfFont == nullptr) // TTF ������ ������ ���
	{
		Fractal::Debug::GetInstance()->ConsoleError(std::string(TTF_GetError()) + " - Error occurred during creation of TTF font!");
	}

	// GPU �� �ؽ��� ���� ��û�ϰ� ����� �ؽ��� ID ��ȯ�ޱ�
	glGenTextures(1, &ttfTextureId);
}


/* - SDL_TTF �� ���� �ϴ� �Լ� | Destroy the SDL_TTF - */
void Fractal::SpriteFont::Destroy()
{
	glDeleteTextures(GL_TEXTURE_2D, &ttfTextureId); // GL �ؽ��� ����
	SDL_FreeSurface(ttfSurface); // SDL �ؽ��� ����
	TTF_CloseFont(ttfFont); // ��Ʈ ����
	TTF_Quit(); // TTF ����
}


/* - ����� ������ �ۼ��ϴ� �Լ� (�ۼ��� ����) | Write text content to be output (Content string) - */
void Fractal::SpriteFont::TextWrite(std::string newString)
{
	// �ؽ�Ʈ ������ ������ ���Ͽ� �ٲ� ��� ����� �ؽ��� �ٽ� �ۼ�
	if (newString != oldString)
	{
		// ��Ƽ����Ʈ ���ڿ��� �����ڵ�� ��ȯ
		wchar_t* unicodeText = new wchar_t[newString.length() + 1]; // �����ڵ� ����� ���� ����
		size_t convertedChars = 0; // ��ȯ�� ���� �� ī����
		mbstowcs_s(&convertedChars, unicodeText, newString.length() + 1, newString.c_str(), newString.length()); // MBCS(Multi Byte Character System) -> WBCS(Wide Byte Character System)

		// SDL �ؽ��ķ� ����
		ttfSurface = TTF_RenderUNICODE_Blended(ttfFont, (uint16_t *)unicodeText, baseColor);

		if (ttfSurface == nullptr) // SDL �ؽ��� ������ ������ ���
		{
			Fractal::Debug::GetInstance()->ConsoleError(std::string(TTF_GetError()) + " - Error occurred during creation of TTF texture!");
		}

		// �ۼ��� �ؽ��� �Ҵ�ޱ�
		glBindTexture(GL_TEXTURE_2D, ttfTextureId);

		// �Ӹ� ���� ���� �ؽ��Ŀ� �׸���
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ttfSurface->w, ttfSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ttfSurface->pixels);

		// �ۼ��� ������ �Ҵ���� �ؽ��� ����
		glBindTexture(GL_TEXTURE_2D, 0);

		// �ؽ�Ʈ ������ ���ϴ��� ���ϱ� ���� ����
		oldString = newString;
	}
}


/* - �ۼ��� ������ ����ϴ� �Լ� (��ġ, ��ġ, ũ��, ��) | Show text (Batch ID, Position, Scale, Color) - */
void Fractal::SpriteFont::TextShow(SpriteBatch& newBatch, glm::vec2 newPosition, glm::vec2 newScale, Vertex::ColorRGBA8 newColor)
{
	glm::vec4 posAndSize = glm::vec4(newPosition.x, newPosition.y, newScale.x, newScale.y); // ��ġ �� ũ�⸦ vec4�� ��ȯ
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f); // UV �簢�� (UV ��ǥ)

	newBatch.Draw(posAndSize, uv, ttfTextureId, 0.0f, newColor); // ��ġ ������ ����
}
