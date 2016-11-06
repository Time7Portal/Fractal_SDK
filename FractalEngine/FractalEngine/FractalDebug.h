#pragma once
#include <GL\glew.h> // http://glew.sourceforge.net/
#include <iostream>
#include <string>
#include <cstdlib>
#include <direct.h>
#define _DEBUGER_INFO_ "[Console] # Fractal Debug Console v3.2 #\n"


namespace Fractal
{
	class Debug
	{
	public:
		static Debug* GetInstance(); // ����� Ŭ������ ������ ���� ��ȯ�ϴ� �Լ� (�̱��� ������) | Return the debug class instance
		static void FreeInstance(); // ����� Ŭ������ �����͸� �����ϴ� �Լ� | Delete the debug class instance

		bool showFps = false; // FPS ǥ�� ���� | Show FPS in title bar window

		std::string GetProgramPath(); // �������� ���α׷� ��θ� �������� �Լ� | Get program path
		void ConsoleGLInfo(); // �׷��� ī�� ������ ����ϴ� �Լ� | Show graphics card information
		void ConsoleDebug(); // ������ �ܼ��� ���� �Լ� | Open debug console
		void ConsoleError(std::string errorString); // �ֿܼ� ������ ǥ���ϴ� �Լ� | Show fatal error in Console (Error message)

	private:
		Debug(); // Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start
		~Debug(); // Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end

		static Debug* debugInstance; // �̱��� ���� ������ ���� ���� ������ | Static pointer for singleton pattern debug thread
	};
}
