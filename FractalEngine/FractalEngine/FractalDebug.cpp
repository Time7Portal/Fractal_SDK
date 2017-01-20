#include "FractalDebug.h"


// �̱��� ���� ������ ���� ���� ������ ������ �̸� ���� �� �ʱ�ȭ (Static pointer for singleton pattern debug thread)
Fractal::Debug* Fractal::Debug::debugInstance = NULL;


/* - Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start - */
Fractal::Debug::Debug()
{
}


/* - Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end - */
Fractal::Debug::~Debug()
{
}


/* - ����� Ŭ������ ������ ���� ��ȯ�ϴ� �Լ� (�̱��� ������) | Return the debug class instance - */
Fractal::Debug* Fractal::Debug::GetInstance()
{
	if (NULL == debugInstance) // �����Ͱ� NULL�� ���
	{
		debugInstance = new Debug(); // �� ������ ���� �Ҵ� �� �����Ϳ� ����
	}
	return debugInstance; // ������ ����
}


/* - ����� Ŭ������ �����͸� �����ϴ� �Լ� | Delete the debug class instance - */
void Fractal::Debug::FreeInstance()
{
	if (NULL != debugInstance) // �����Ͱ� ������� �ʴٸ�
	{
		delete debugInstance; // �޸� ����
		debugInstance = NULL;
	}
}


/* - �ܼ� ���(�ѱ� ����)�� ���� ��Ƽ����Ʈ ���ڿ��� �����ڵ�� ��ȯ�ϴ� �Լ� | Convert multibyte strings to Unicode for console output - */
std::wstring Fractal::Debug::ChangeToWstring(const std::string newString)
{
	// ��Ƽ����Ʈ ���ڿ��� �����ڵ�� ��ȯ
	wchar_t* widebyteString = new wchar_t[newString.length() + 1]; // �����ڵ� ����� ���� ����
	size_t convertedChars = 0; // ��ȯ�� ���� �� ī����
	mbstowcs_s(&convertedChars, widebyteString, newString.length() + 1, newString.c_str(), newString.length()); // MBCS(Multi Byte Character System) -> WBCS(Wide Byte Character System)
	return widebyteString;
}


/* - �������� ���α׷� ��θ� �������� �Լ� | Get program path - */
std::string Fractal::Debug::GetProgramPath(void)
{
	char* filePathBuffer; // �ӽ� ���ϰ�� ����
	int cutPosition; // �ӽ� ���ϰ�� ������ �ڸ� ���� ��ġ
	std::string filePath; // ���� ���ϰ��
	std::string cutPath = "FractalEngine"; // �ڸ� ���ڿ�(����)

	// ���� ��� �޾ƿ���
	if ((filePathBuffer = _getcwd(NULL, 0)) == NULL)
	{
		ConsoleError("Unable to get the program path from _getcwd...");
	}
	else
	{
		// ���� ���۸� ��Ʈ������ ����
		filePath = std::string(filePathBuffer);

		// �ڸ� ���ڿ�(������ġ)�� ���° ���ں��� �����ϴ��� ��ġ ���
		cutPosition = (int)filePath.find(cutPath);
		cutPosition += (int)cutPath.length();

		// ù��° ������ġ ������ ����� ������ ��� ����
		filePath.erase(cutPosition, filePath.length() - cutPosition);

		// ���� ����
		free(filePathBuffer);

		// _getcwd�� ������ ���� ��� ǥ��
		WriteConsoleMessage("Extraction the program path : \n" + filePath);
	}

	return filePath; // ���� ���ϰ�� ��ȯ
}


/* - �׷��� ī�� ������ ����ϴ� �Լ� | Show graphics card information - */
void Fractal::Debug::ConsoleGLInfo()
{
	std::wcout << L"[Console] Graphics card info : " << std::endl;
	std::cout << glGetString(GL_VENDOR) << " | " << glGetString(GL_RENDERER) << " | " << glGetString(GL_VERSION) << std::endl;
}


/* - ������ �ܼ��� ���� �Լ� | Open debug console - */
void Fractal::Debug::ConsoleOpen()
{
	while (1)
	{
		// ���� ǥ��
		std::wcout << _DEBUGER_INFO_ << std::endl;
		std::wcout << L"[Console] Write 0 and press the enter key to exit the program..." << std::endl;
		std::wcout << L"[Console] Write 1 and press the enter key to open the help ..." << std::endl;
		std::wcout << L"[Console] Write 2 and press the enter key to display the FPS count..." << std::endl;

		int toConsole = 255; // �ܼ� ����� �ޱ� ���� ����
		std::cin >> toConsole; // �ܼ� ��ɾ� �ޱ�

		if (toConsole == 0) // ���� �ܼ� ��ɾ�� 0�̶�� ���ڸ� �޾�����
		{
			exit(0); // ���α׷� ���� ����
		}
		else if (toConsole == 1) // ���� �ܼ� ��ɾ�� 1�̶�� ���ڸ� �޾�����
		{
			continue; // while�� �ٽ� ���� (���� ǥ��)
		}
		else if (toConsole == 2) // ���� �ܼ� ��ɾ�� 2�̶�� ���ڸ� �޾�����
		{
			if (showFps == false)
			{
				showFps = true; // FPS ǥ�� Ȱ��ȭ
				std::wcout << L"[Console] FPS counter enabled..." << std::endl;
			}
			else
			{
				showFps = false; // FPS ǥ��	��Ȱ��ȭ
				std::wcout << L"[Console] FPS counter disabled..." << std::endl;
			}
		}
		else // ���� �ܼ� ��ɾ�� �߸��� ���� �޾�����
		{
			std::wcout << L"[Console] Invalid command..." << std::endl;

			// �߸��� ����� ���� ��� ���� ����
			std::cin.clear(); // cin ���� �ʱ�ȭ
			std::cin.ignore(); // �Է� ���� �ʱ�ȭ
		}
	}
}


/* - ������ �޼����� ����ϴ� �Լ� | Output debug information in debug console - */
void Fractal::Debug::WriteConsoleMessage(const std::string newString)
{
	// �ܼ� ���(�ѱ� ����)�� ���� ��Ƽ����Ʈ ���ڿ��� �����ڵ�� ��ȯ
	std::wstring w_String = ChangeToWstring(newString);

	// �޼��� ���
	std::wcout << L"[Console] " << w_String << std::endl;
}


/* - �ֿܼ� ������ ǥ���ϴ� �Լ� | Show fatal error in console - */
void Fractal::Debug::ConsoleError(const std::string newErrorString)
{
	// �ܼ� ���(�ѱ� ����)�� ���� ��Ƽ����Ʈ ���ڿ��� �����ڵ�� ��ȯ
	std::wstring w_ErrorString = ChangeToWstring(newErrorString);

	// ����Ʈ���� ���� ���� ���
	std::wcout << L"[App Error] : " << w_ErrorString << std::endl;

	// �ý��� ���� ���� ���
	std::perror("[System Error] ");
	std::wcout << L"[Console] Fatal error has occurred. Press any key to close this Program..." << std::endl;

	// �ܼ� �ӽ� ���
	std::system("pause");

	exit(1); // ���α׷� ������ ����
}
