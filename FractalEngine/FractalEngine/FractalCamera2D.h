#pragma once
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>


namespace Fractal
{
	class Camera2D
	{
	public:
		Camera2D(); // 클래스 시작시 호출되는 함수 | Function called when the class start
		~Camera2D(); // 클래스 종료시 호출되는 함수 | Function called when the class end

		void Init(int newScreenWidth, int newScreenHeight); // 카메라를 초기화 하는 함수 | Sets up the Orthographic matrix and Screen dimensions

		void Update(); // 카메라 좌표를 업데이트 하는 함수 | Updates the camera matrix if needed

		glm::vec2 ConvertScreenToWorld(glm::vec2 screenCoords); // 스크린 좌표를 월드 좌표로 변환하는 함수 | Convert the Screen coordinate to World

		void SetPosition(const glm::vec2& newPosition); // 카메라 위치를 설정하는 함수 | Set Camera Position

		void SetScale(float newScale); // 카메라 줌을 설정하는 함수 | Set Camera Scale 

		// 카메라 속성값을 반환하는 함수들 | Get Camera attributes
		glm::vec2 GetPosition(){ return position; }
		float GetScale(){ return scale; }
		glm::mat4 GetCameraMatrix() { return cameraMatrix; }

	private:
		int screenWidth, screenHeight; // 스크린 넓이, 높이 | Screen size
		bool needsMatrixUpdate; // 카메라 속성이 변하여 업데이트가 필요한지 | Needs camera Matrix update
		float scale; // 카메라 뷰 크기 | Camera scale
		glm::vec2 position; // 카메라 위치 | Camera position
		glm::mat4 orthoMatrix; // 카메라 투영법 | Camera projection mode
		glm::mat4 cameraMatrix; // 카메라 좌표값 | Camera matrix
	};
}
