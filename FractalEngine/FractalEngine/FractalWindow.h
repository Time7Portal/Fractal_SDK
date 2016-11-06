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

		void SetWindowName(std::string windowName); // ������ �̸� ���� | Set SDL Window name

		void CalculateFPS(); // FPS ��� | Calculate FPS

		float GetFPS(); // FPS �� �ޱ� | Get FPS value

		void SwapBuffer(); // OpenGL ���� ��ü | Swap our buffer and draw everything to the screen!

	private:
		SDL_Window* _sdlWindow; // SDL ������ | SDL Window

		//FPS ǥ�ø� ���� ����
		Uint32 FRAME_SAMPLES = 8; // �ε巯�� FPS ����� ���� ������ ǥ�� | How many frames to keep smoother the result
		Uint32 frametimes[8]; // ǥ�� ����� �迭 | An array to store frame times : FRAME_SAMPLES 8
		Uint32 frametimelast = SDL_GetTicks(); // ���������� ���� ������ �ð� | Last calculated SDL_GetTicks
		Uint32 framecount; // �� ������ ������ �� | total frames rendered
		float nowFPS; // ���� ���� �ǽð�FPS | RealTime FPS
	};
}