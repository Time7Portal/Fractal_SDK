#pragma once
#include <unordered_map>
#include <GLM\glm.hpp>


namespace Fractal
{
	class InputManager
	{
	public:
		InputManager(); // 클래스 시작시 호출되는 함수 | Function called when the class start
		~InputManager(); // 클래스 종료시 호출되는 함수 | Function called when the class end

		void PressKey(unsigned int keyID); // 해당 키를 누른 상태로 바꾸는 함수 | Memory the pressed key
		void ReleaseKey(unsigned int keyID); // 해당 키를 뗀 상태로 바꾸는 함수 | Memory the released key

		void SetMouseCoords(unsigned int x, unsigned int y); // 마우스 좌표 받기 | Memory the mouse coordinate
		glm::vec2 GetMouseCoords() const { return mouseCoords; } // 마우스 좌표 넘기기 | Give the mouse coordinate

		bool IsKeyPressed(unsigned int keyID); // 해당 키가 눌렸는지 확인 | Check the pressed key
		
	private:
		std::unordered_map<unsigned int, bool> keyMap; // 키 배열 | Key map array
		glm::vec2 mouseCoords; // 마우스 좌표 | Mouse coordinate
	};
}