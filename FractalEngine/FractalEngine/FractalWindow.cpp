#include "FractalWindow.h"


Fractal::Window::Window()
{
}


Fractal::Window::~Window()
{
}

int Fractal::Window::Create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
{
	// SDL ���̺귯�� �ʱ�ȭ (SDL ����� ����ϱ� ���� ������� ȣ��) | Initialize SDL
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

	// SDL ������ â ���� | Open an SDL window
	_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);

	if (_sdlWindow == nullptr) // �����찡 ������
	{
		Fractal::Debug::GetInstance()->ConsoleError("SDL window is not opened!");
	}

	// OpenGL ���ؽ�Ʈ�� SDL�� ���� | Set up our OpenGL context
	SDL_GLContext glContex = SDL_GL_CreateContext(_sdlWindow);
	if (glContex == nullptr)
	{
		Fractal::Debug::GetInstance()->ConsoleError("OpenGL context is not connected to the SDL!");
	}

	// GLEW �ʱ�ȭ Ȯ�� | Set up GLEW
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		Fractal::Debug::GetInstance()->ConsoleError("GLEW is not initialized!");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // ������۸� Ȱ��ȭ | Tell SDL that we want a double buffered window so we don't get any flickering
	SDL_GL_SetSwapInterval(1); // Vsync(��������) Ȱ��ȭ

	// ���İ� ���� | Enable alpha blend
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

	frametimesindex = framecount % FRAME_SAMPLES; // ������ ���� ȸ�� | FRAME_SAMPLES cycle
	nowTicks = SDL_GetTicks(); // ���� SDL �ð� ���� | Store the current time	
	frametimes[frametimesindex] = nowTicks - frametimelast; // ������ Ÿ�� ���� | Save the frame time value
	frametimelast = nowTicks; // ���� ����� ���� ������ ������ �ð� ���� | Save the last frame time for the next calculate FPS
	framecount++; // ƽ ����ø��� ������ ī��Ʈ ���� | Increment the frame count

	// ���� ������ ��� ���� | Work out the current framerate
	if (framecount < FRAME_SAMPLES)
	{
		count = framecount;
	}
	else
	{
		count = FRAME_SAMPLES;
	}

	// ��� ������ �ð��� ���� | Add up all the values and divide to get the average frame time
	nowFPS = 0;
	for (i = 0; i < count; i++)
	{
		nowFPS += frametimes[i];
	}
	nowFPS /= count;

	// ���� ������ ���� | Now to make it an actual frames per second value
	nowFPS = 1000.f / nowFPS;
}

float Fractal::Window::GetFPS()
{
	return nowFPS;
}

void Fractal::Window::SwapBuffer()
{
	SDL_GL_SwapWindow(_sdlWindow); // OpenGL ���� ��ü | Swap our buffer and draw everything to the screen!
}