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
		static Debug* GetInstance(); // 디버그 클래스의 포인터 값을 반환하는 함수 (싱글톤 스레드) | Return the debug class instance
		static void FreeInstance(); // 디버그 클래스의 포인터를 제거하는 함수 | Delete the debug class instance

		bool showFps = false; // FPS 표시 여부 | Show FPS in title bar window

		std::string GetProgramPath(); // 실행중인 프로그램 경로를 가져오는 함수 | Get program path
		void ConsoleGLInfo(); // 그래픽 카드 정보를 출력하는 함수 | Show graphics card information
		void ConsoleDebug(); // 디버깅용 콘솔을 띄우는 함수 | Open debug console
		void ConsoleError(std::string errorString); // 콘솔에 오류를 표시하는 함수 | Show fatal error in Console (Error message)

	private:
		Debug(); // 클래스 시작시 호출되는 함수 | Function called when the class start
		~Debug(); // 클래스 종료시 호출되는 함수 | Function called when the class end

		static Debug* debugInstance; // 싱글톤 패턴 구현을 위한 정적 포인터 | Static pointer for singleton pattern debug thread
	};
}
