#pragma once
#include <unordered_map>
#include <GLM\glm.hpp>


namespace Fractal
{
	class InputManager
	{
	public:
		InputManager(); // Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start
		~InputManager(); // Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end

		void PressKey(unsigned int keyID); // �ش� Ű�� ���� ���·� �ٲٴ� �Լ� | Memory the pressed key
		void ReleaseKey(unsigned int keyID); // �ش� Ű�� �� ���·� �ٲٴ� �Լ� | Memory the released key

		void SetMouseCoords(unsigned int x, unsigned int y); // ���콺 ��ǥ �ޱ� | Memory the mouse coordinate
		glm::vec2 GetMouseCoords() const { return mouseCoords; } // ���콺 ��ǥ �ѱ�� | Give the mouse coordinate

		bool IsKeyPressed(unsigned int keyID); // �ش� Ű�� ���ȴ��� Ȯ�� | Check the pressed key
		
	private:
		std::unordered_map<unsigned int, bool> keyMap; // Ű �迭 | Key map array
		glm::vec2 mouseCoords; // ���콺 ��ǥ | Mouse coordinate
	};
}