#include "FractalDebug.h"


// 싱글톤 패턴 구현을 위한 정적 포인터 변수를 미리 정의 후 초기화 (Static pointer for singleton pattern debug thread)
Fractal::Debug* Fractal::Debug::debugInstance = NULL;


/* - 클래스 시작시 호출되는 함수 | Function called when the class start - */
Fractal::Debug::Debug()
{
}


/* - 클래스 종료시 호출되는 함수 | Function called when the class end - */
Fractal::Debug::~Debug()
{
}


/* - 디버그 클래스의 포인터 값을 반환하는 함수 (싱글톤 스레드) | Return the debug class instance - */
Fractal::Debug* Fractal::Debug::GetInstance()
{
	if (NULL == debugInstance) // 포인터가 NULL일 경우
	{
		debugInstance = new Debug(); // 힙 영역에 동적 할당 후 포인터에 저장
	}
	return debugInstance; // 포인터 리턴
}


/* - 디버그 클래스의 포인터를 제거하는 함수 | Delete the debug class instance - */
void Fractal::Debug::FreeInstance()
{
	if (NULL != debugInstance) // 포인터가 비어있지 않다면
	{
		delete debugInstance; // 메모리 해제
		debugInstance = NULL;
	}
}


/* - 콘솔 출력(한글 지원)을 위해 멀티바이트 문자열을 유니코드로 변환하는 함수 | Convert multibyte strings to Unicode for console output - */
std::wstring Fractal::Debug::ChangeToWstring(const std::string newString)
{
	// 멀티바이트 문자열을 유니코드로 변환
	wchar_t* widebyteString = new wchar_t[newString.length() + 1]; // 유니코드 저장용 변수 지정
	size_t convertedChars = 0; // 변환된 문자 수 카운터
	mbstowcs_s(&convertedChars, widebyteString, newString.length() + 1, newString.c_str(), newString.length()); // MBCS(Multi Byte Character System) -> WBCS(Wide Byte Character System)
	return widebyteString;
}


/* - 실행중인 프로그램 경로를 가져오는 함수 | Get program path - */
std::string Fractal::Debug::GetProgramPath(void)
{
	char* filePathBuffer; // 임시 파일경로 버퍼
	int cutPosition; // 임시 파일경로 버퍼의 자를 문자 위치
	std::string filePath; // 최종 파일경로
	std::string cutPath = "FractalEngine"; // 자를 문자열(폴더)

	// 파일 경로 받아오기
	if ((filePathBuffer = _getcwd(NULL, 0)) == NULL)
	{
		ConsoleError("Unable to get the program path from _getcwd...");
	}
	else
	{
		// 최종 버퍼를 스트링으로 저장
		filePath = std::string(filePathBuffer);

		// 자를 문자열(폴더위치)이 몇번째 문자부터 시작하는지 위치 기억
		cutPosition = (int)filePath.find(cutPath);
		cutPosition += (int)cutPath.length();

		// 첫번째 폴더위치 까지만 남기고 나머지 경로 제거
		filePath.erase(cutPosition, filePath.length() - cutPosition);

		// 버퍼 비우기
		free(filePathBuffer);

		// _getcwd로 추출한 파일 경로 표시
		WriteConsoleMessage("Extraction the program path : \n" + filePath);
	}

	return filePath; // 최종 파일경로 반환
}


/* - 그래픽 카드 정보를 출력하는 함수 | Show graphics card information - */
void Fractal::Debug::ConsoleGLInfo()
{
	std::wcout << L"[Console] Graphics card info : " << std::endl;
	std::cout << glGetString(GL_VENDOR) << " | " << glGetString(GL_RENDERER) << " | " << glGetString(GL_VERSION) << std::endl;
}


/* - 디버깅용 콘솔을 띄우는 함수 | Open debug console - */
void Fractal::Debug::ConsoleOpen()
{
	while (1)
	{
		// 도움말 표시
		std::wcout << _DEBUGER_INFO_ << std::endl;
		std::wcout << L"[Console] Write 0 and press the enter key to exit the program..." << std::endl;
		std::wcout << L"[Console] Write 1 and press the enter key to open the help ..." << std::endl;
		std::wcout << L"[Console] Write 2 and press the enter key to display the FPS count..." << std::endl;

		int toConsole = 255; // 콘솔 명령을 받기 위한 변수
		std::cin >> toConsole; // 콘솔 명령어 받기

		if (toConsole == 0) // 만약 콘솔 명령어로 0이라는 숫자를 받았을때
		{
			exit(0); // 프로그램 정상 종료
		}
		else if (toConsole == 1) // 만약 콘솔 명령어로 1이라는 숫자를 받았을때
		{
			continue; // while문 다시 시작 (도움말 표시)
		}
		else if (toConsole == 2) // 만약 콘솔 명령어로 2이라는 숫자를 받았을때
		{
			if (showFps == false)
			{
				showFps = true; // FPS 표시 활성화
				std::wcout << L"[Console] FPS counter enabled..." << std::endl;
			}
			else
			{
				showFps = false; // FPS 표시	비활성화
				std::wcout << L"[Console] FPS counter disabled..." << std::endl;
			}
		}
		else // 만약 콘솔 명령어로 잘못된 값을 받았을때
		{
			std::wcout << L"[Console] Invalid command..." << std::endl;

			// 잘못된 명령을 받은 경우 버퍼 비우기
			std::cin.clear(); // cin 상태 초기화
			std::cin.ignore(); // 입력 버퍼 초기화
		}
	}
}


/* - 디버깅용 메세지를 출력하는 함수 | Output debug information in debug console - */
void Fractal::Debug::WriteConsoleMessage(const std::string newString)
{
	// 콘솔 출력(한글 지원)을 위해 멀티바이트 문자열을 유니코드로 변환
	std::wstring w_String = ChangeToWstring(newString);

	// 메세지 출력
	std::wcout << L"[Console] " << w_String << std::endl;
}


/* - 콘솔에 오류를 표시하는 함수 | Show fatal error in console - */
void Fractal::Debug::ConsoleError(const std::string newErrorString)
{
	// 콘솔 출력(한글 지원)을 위해 멀티바이트 문자열을 유니코드로 변환
	std::wstring w_ErrorString = ChangeToWstring(newErrorString);

	// 소프트웨어 오류 내용 출력
	std::wcout << L"[App Error] : " << w_ErrorString << std::endl;

	// 시스템 오류 내용 출력
	std::perror("[System Error] ");
	std::wcout << L"[Console] Fatal error has occurred. Press any key to close this Program..." << std::endl;

	// 콘솔 임시 대기
	std::system("pause");

	exit(1); // 프로그램 비정상 종료
}
