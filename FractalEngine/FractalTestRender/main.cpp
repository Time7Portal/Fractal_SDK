#include "FractalGame.h"


int main(int argc, char** argv)
{
	setlocale(LC_ALL, ""); // ���α׷� ������(���) ����

	MyGame::Game mainGame; // ���� ���� ���� �ν��Ͻ� ����

	mainGame.Run(); // ���� ����

	return 0; // ���α׷� ���� ����
}
