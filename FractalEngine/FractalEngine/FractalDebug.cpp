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
		std::cout << "[Console] Unable to get the program path from _getcwd..." << std::endl;
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

		std::cout << "[Console] Extraction the program path : " << filePath << std::endl;
	}

	return filePath; // ���� ���ϰ�� ��ȯ
}


/* - �׷��� ī�� ������ ����ϴ� �Լ� | Show graphics card information - */
void Fractal::Debug::ConsoleGLInfo()
{
	std::cout << "[Console] Graphics card info : " << std::endl;
	std::cout << glGetString(GL_VENDOR) << " | " << glGetString(GL_RENDERER) << " | " << glGetString(GL_VERSION) << std::endl;
}


/* - ������ �ܼ��� ���� �Լ� | Open debug console - */
void Fractal::Debug::ConsoleDebug()
{
	while (1)
	{
		// ���� ǥ��
		std::cout << _DEBUGER_INFO_ << std::endl;
		std::cout << "[Console] Write 0 and press the enter key to exit the program..." << std::endl;
		std::cout << "[Console] Write 1 and press the enter key to open the help ..." << std::endl;
		std::cout << "[Console] Write 2 and press the enter key to display the FPS count..." << std::endl;

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
				std::cout << "[Console] FPS counter enabled..." << std::endl;
			}
			else
			{
				showFps = false; // FPS ǥ��	��Ȱ��ȭ
				std::cout << "[Console] FPS counter disabled..." << std::endl;
			}
		}
		else // ���� �ܼ� ��ɾ�� �߸��� ���� �޾�����
		{
			std::cout << "[Console] Invalid command..." << std::endl;

			// �߸��� ����� ���� ��� ���� ����
			std::cin.clear(); // cin ���� �ʱ�ȭ
			std::cin.ignore(); // �Է� ���� �ʱ�ȭ
		}
	}
}


/* - �ֿܼ� ������ ǥ���ϴ� �Լ� | Show fatal error in console - */
void Fractal::Debug::ConsoleError(std::string errorString)
{
	std::cout << "[Console Error] : " << errorString << std::endl; // ����Ʈ���� ���� ���� ���
	std::perror("[System Error] "); // �ý��� ���� ���� ���
	std::cout << "[Console] Fatal error has occurred. Press any key to close..." << std::endl;
	std::system("pause"); // �ܼ� �ӽ� ���
	exit(1); // ���α׷� ������ ����
}
