#include "FractalWindow.h"


Fractal::Window::Window()
{
}


Fractal::Window::~Window()
{
}

int Fractal::Window::Create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
{
	// SDL 라이브러리 초기화 (SDL 기능을 사용하기 전에 가장먼저 호출) | Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	Uint32 flags = SDL_WINDOW_OPENGL;

	if (currentFlags & INVISIBLE)
	{
		flags |= SDL_WINDOW_HIDDEN;
	}
	if (currentFlags & WINDOWED)
	{
		flags |= SDL_WINDOW_SHOWN;
	}
	if (currentFlags & BORDERLESS)
	{
		flags |= SDL_WINDOW_BORDERLESS;
	}
	if (currentFlags & FULLSCREEN)
	{
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	// SDL 윈도우 창 열기 | Open an SDL window
	_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);

	if (_sdlWindow == nullptr) // 윈도우가 없으면
	{
		Fractal::Debug::GetInstance()->ConsoleError("SDL window is not opened!");
	}

	// OpenGL 콘텍스트를 SDL에 연결 | Set up our OpenGL context
	SDL_GLContext glContex = SDL_GL_CreateContext(_sdlWindow);
	if (glContex == nullptr)
	{
		Fractal::Debug::GetInstance()->ConsoleError("OpenGL context is not connected to the SDL!");
	}

	// GLEW 초기화 확인 | Set up GLEW
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		Fractal::Debug::GetInstance()->ConsoleError("GLEW is not initialized!");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // 더블버퍼링 활성화 | Tell SDL that we want a double buffered window so we don't get any flickering
	SDL_GL_SetSwapInterval(1); // Vsync(수직동기) 활성화

	// 알파값 적용 | Enable alpha blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void Fractal::Window::SetWindowName(std::string windowName)
{
	SDL_SetWindowTitle(_sdlWindow, windowName.c_str());
}

void Fractal::Window::CalculateFPS()
{
	static Uint32 frametimesindex;
	static Uint32 nowTicks;
	static Uint32 count;
	static Uint32 i;

	frametimesindex = framecount % FRAME_SAMPLES; // 프레임 샘플 회전 | FRAME_SAMPLES cycle
	nowTicks = SDL_GetTicks(); // 현재 SDL 시각 저장 | Store the current time	
	frametimes[frametimesindex] = nowTicks - frametimelast; // 프레임 타임 저장 | Save the frame time value
	frametimelast = nowTicks; // 다음 계산을 위해 마지막 프레임 시각 저장 | Save the last frame time for the next calculate FPS
	framecount++; // 틱 실행시마다 프레임 카운트 증가 | Increment the frame count

	// 현재 프레임 계산 시작 | Work out the current framerate
	if (framecount < FRAME_SAMPLES)
	{
		count = framecount;
	}
	else
	{
		count = FRAME_SAMPLES;
	}

	// 평균 프레임 시간을 구함 | Add up all the values and divide to get the average frame time
	nowFPS = 0;
	for (i = 0; i < count; i++)
	{
		nowFPS += frametimes[i];
	}
	nowFPS /= count;

	// 실제 프레임 산출 | Now to make it an actual frames per second value
	nowFPS = 1000.f / nowFPS;
}

float Fractal::Window::GetFPS()
{
	return nowFPS;
}

void Fractal::Window::SwapBuffer()
{
	SDL_GL_SwapWindow(_sdlWindow); // OpenGL 버퍼 교체 | Swap our buffer and draw everything to the screen!
}