#include "FractalCamera2D.h"


/* - 클래스 시작시 호출되는 함수 | Function called when the class start - */
Fractal::Camera2D::Camera2D()
{
	needsMatrixUpdate = true;
	screenWidth = 0;
	screenHeight = 0;
	scale = 1.0f;
	position = glm::vec2(0.0f, 0.0f);
	cameraMatrix = glm::mat4(1.0f);
	orthoMatrix = glm::mat4(1.0f);
}


/* - 클래스 종료시 호출되는 함수 | Function called when the class end - */
Fractal::Camera2D::~Camera2D()
{
}


/* - 카메라를 초기화 하는 함수 | Sets up the Orthographic matrix and Screen dimensions - */
void Fractal::Camera2D::Init(int newScreenWidth, int newScreenHeight)
{
	screenWidth = newScreenWidth;
	screenHeight = newScreenHeight;
	orthoMatrix = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight);
}


/* - 카메라 좌표를 업데이트 하는 함수 | Updates the camera matrix if needed - */
void Fractal::Camera2D::Update()
{
	if (needsMatrixUpdate)
	{
		// 카메라 위치 설정 | Set camera translation
		glm::vec3 translate(position.x + screenWidth / 2, position.y + screenHeight / 2, 0.0f);
		cameraMatrix = glm::translate(orthoMatrix, translate);

		// 카메라 크기 설정 | Set camera scale
		glm::vec3 scale(scale, scale, 0.0f);
		cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * cameraMatrix;

		needsMatrixUpdate = false;
	}
}


/* - 카메라 위치를 설정하는 함수 | Set Camera Position - */
void Fractal::Camera2D::SetPosition(const glm::vec2& newPosition)
{
	position = newPosition;
	needsMatrixUpdate = true; // 카메라 속성이 변경되어 업데이트 필요성 활성화
}


/* -  카메라 줌을 설정하는 함수 | Set Camera Scale - */
void Fractal::Camera2D::SetScale(float newScale)
{
	// 카메라 줌 값이 음수가 되지 않도록 함
	if (newScale > 0)
	{
		scale = newScale;
		needsMatrixUpdate = true;
	}
}


/* - 스크린 좌표를 월드 좌표로 변환하는 함수 | Convert the Screen coordinate to World - */
glm::vec2 Fractal::Camera2D::ConvertScreenToWorld(glm::vec2 screenCoords)
{
	// 중앙을 0점으로 설정 | Make it so that 0 is the center
	screenCoords = glm::vec2(screenCoords.x - screenWidth / 2, -(screenCoords.y - screenHeight / 2));
	// 스케일 정규화 | Scale the coordinates
	screenCoords /= scale;
	// 카메라 포지션을 전환 | Translate with the camera position
	screenCoords -= position;

	return screenCoords;
}
