#pragma once
#include <GL\glew.h> // http://glew.sourceforge.net/
#include <iostream>
#include <string>
#include <cstdlib>
#include <direct.h>
#define _DEBUGER_INFO_ L"[Console] # Fractal Debug Console v3.5 #"


namespace Fractal
{
	class Debug
	{
	public:
		static Debug* GetInstance(); // ����� Ŭ������ ������ ���� ��ȯ�ϴ� �Լ� (�̱��� ������) | Return the debug class instance
		static void FreeInstance(); // ����� Ŭ������ �����͸� �����ϴ� �Լ� | Delete the debug class instance

		bool showFps = false; // FPS ǥ�� ���� | Show FPS in title bar window

		std::wstring ChangeToWstring(const std::string newString); // �ܼ� ���(�ѱ� ����)�� ���� ��Ƽ����Ʈ ���ڿ��� �����ڵ�� ��ȯ�ϴ� �Լ� | Convert multibyte strings to Unicode for console output
		std::string GetProgramPath(); // �������� ���α׷� ��θ� �������� �Լ� | Get program path
		void ConsoleGLInfo(); // �׷��� ī�� ������ ����ϴ� �Լ� | Show graphics card information
		void ConsoleOpen(); // ������ �ܼ��� ���� �Լ� | Open debug console
		void WriteConsoleMessage(const std::string newString); // ������ �޼����� ����ϴ� �Լ� | Output debug information in debug console
		void ConsoleError(const std::string newErrorString); // �ֿܼ� ������ ǥ���ϴ� �Լ� | Show fatal error in Console (Error message)

	private:
		Debug(); // Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start
		~Debug(); // Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end

		static Debug* debugInstance; // �̱��� ���� ������ ���� ���� ������ | Static pointer for singleton pattern debug thread
	};
}
