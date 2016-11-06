#pragma once
#include <string>
#include <SDL\SDL.h> //http://www.libsdl.org/
#include <GL\glew.h> //http://glew.sourceforge.net/
#include "FractalDebug.h"


namespace Fractal
{
	enum WindowFlags { INVISIBLE = 0x1, WINDOWED = 0x2, BORDERLESS = 0x4, FULLSCREEN = 0x8 };


	class Window
	{
	public:
		Window();
		~Window();

		int Create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);

		void SetWindowName(std::string windowName); // 윈도우 이름 변경 | Set SDL Window name

		void CalculateFPS(); // FPS 계산 | Calculate FPS

		float GetFPS(); // FPS 값 받기 | Get FPS value

		void SwapBuffer(); // OpenGL 버퍼 교체 | Swap our buffer and draw everything to the screen!

	private:
		SDL_Window* _sdlWindow; // SDL 윈도우 | SDL Window

		//FPS 표시를 위한 변수
		Uint32 FRAME_SAMPLES = 8; // 부드러운 FPS 계산을 위한 프레임 표본 | How many frames to keep smoother the result
		Uint32 frametimes[8]; // 표본 저장용 배열 | An array to store frame times : FRAME_SAMPLES 8
		Uint32 frametimelast = SDL_GetTicks(); // 마지막으로 계산된 프레임 시각 | Last calculated SDL_GetTicks
		Uint32 framecount; // 총 렌더된 프레임 수 | total frames rendered
		float nowFPS; // 최종 계산된 실시간FPS | RealTime FPS
	};
}