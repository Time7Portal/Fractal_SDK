#include "FractalGame.h"


int main(int argc, char** argv)
{
	setlocale(LC_ALL, ""); // ���α׷� ������(���) ����
	std::wcin.imbue(std::locale("")); // ���̵����Ʈ �ܼ� �Է� ������(���) ����
	std::wcout.imbue(std::locale("")); // ���̵����Ʈ �ܼ� ��� ������(���) ����

	MyGame::Game mainGame; // ���� ���� ���� �ν��Ͻ� ����

	mainGame.Run(); // ���� ����

	return 0; // ���α׷� ���� ����
}
