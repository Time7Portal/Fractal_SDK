#include "FractalInputManager.h"


/* - 클래스 시작시 호출되는 함수 | Function called when the class start - */
Fractal::InputManager::InputManager()
{
	mouseCoords = glm::vec2(0.0f, 0.0f);
}


/* - 클래스 종료시 호출되는 함수 | Function called when the class end - */
Fractal::InputManager::~InputManager()
{
}


/* - 해당 키를 누른 상태로 바꾸는 함수 | Memory the pressed key - */
void Fractal::InputManager::PressKey(unsigned int keyID)
{
	keyMap[keyID] = true;
}


/* - 해당 키를 뗀 상태로 바꾸는 함수 | Memory the released key - */
void Fractal::InputManager::ReleaseKey(unsigned int keyID)
{
	keyMap[keyID] = false;
}


/* - 마우스 좌표 받기 | Memory the mouse coordinate - */
void Fractal::InputManager::SetMouseCoords(unsigned int x, unsigned int y)
{
	mouseCoords.x = (float)x;
	mouseCoords.y = (float)y;
}


/* - 해당 키가 눌렸는지 확인 | Check the pressed key - */
bool Fractal::InputManager::IsKeyPressed(unsigned int keyID)
{
	auto it = keyMap.find(keyID);
	if (it != keyMap.end())
	{
		return it->second;
	}
	else
	{
		return false;
	}
}
