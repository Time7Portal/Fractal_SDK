#include "FractalInputManager.h"


/* - Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start - */
Fractal::InputManager::InputManager()
{
	mouseCoords = glm::vec2(0.0f, 0.0f);
}


/* - Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end - */
Fractal::InputManager::~InputManager()
{
}


/* - �ش� Ű�� ���� ���·� �ٲٴ� �Լ� | Memory the pressed key - */
void Fractal::InputManager::PressKey(unsigned int keyID)
{
	keyMap[keyID] = true;
}


/* - �ش� Ű�� �� ���·� �ٲٴ� �Լ� | Memory the released key - */
void Fractal::InputManager::ReleaseKey(unsigned int keyID)
{
	keyMap[keyID] = false;
}


/* - ���콺 ��ǥ �ޱ� | Memory the mouse coordinate - */
void Fractal::InputManager::SetMouseCoords(unsigned int x, unsigned int y)
{
	mouseCoords.x = (float)x;
	mouseCoords.y = (float)y;
}


/* - �ش� Ű�� ���ȴ��� Ȯ�� | Check the pressed key - */
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
