#include "FractalGame.h"


int main(int argc, char** argv)
{
	setlocale(LC_ALL, ""); // 프로그램 로케일(언어) 설정

	MyGame::Game mainGame; // 예시 메인 게임 인스턴스 생성

	mainGame.Run(); // 게임 가동

	return 0; // 프로그램 정상 종료
}
