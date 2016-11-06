#include "FractalCamera2D.h"


/* - Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start - */
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


/* - Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end - */
Fractal::Camera2D::~Camera2D()
{
}


/* - ī�޶� �ʱ�ȭ �ϴ� �Լ� | Sets up the Orthographic matrix and Screen dimensions - */
void Fractal::Camera2D::Init(int newScreenWidth, int newScreenHeight)
{
	screenWidth = newScreenWidth;
	screenHeight = newScreenHeight;
	orthoMatrix = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight);
}


/* - ī�޶� ��ǥ�� ������Ʈ �ϴ� �Լ� | Updates the camera matrix if needed - */
void Fractal::Camera2D::Update()
{
	if (needsMatrixUpdate)
	{
		// ī�޶� ��ġ ���� | Set camera translation
		glm::vec3 translate(position.x + screenWidth / 2, position.y + screenHeight / 2, 0.0f);
		cameraMatrix = glm::translate(orthoMatrix, translate);

		// ī�޶� ũ�� ���� | Set camera scale
		glm::vec3 scale(scale, scale, 0.0f);
		cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * cameraMatrix;

		needsMatrixUpdate = false;
	}
}


/* - ī�޶� ��ġ�� �����ϴ� �Լ� | Set Camera Position - */
void Fractal::Camera2D::SetPosition(const glm::vec2& newPosition)
{
	position = newPosition;
	needsMatrixUpdate = true; // ī�޶� �Ӽ��� ����Ǿ� ������Ʈ �ʿ伺 Ȱ��ȭ
}


/* -  ī�޶� ���� �����ϴ� �Լ� | Set Camera Scale - */
void Fractal::Camera2D::SetScale(float newScale)
{
	// ī�޶� �� ���� ������ ���� �ʵ��� ��
	if (newScale > 0)
	{
		scale = newScale;
		needsMatrixUpdate = true;
	}
}


/* - ��ũ�� ��ǥ�� ���� ��ǥ�� ��ȯ�ϴ� �Լ� | Convert the Screen coordinate to World - */
glm::vec2 Fractal::Camera2D::ConvertScreenToWorld(glm::vec2 screenCoords)
{
	// �߾��� 0������ ���� | Make it so that 0 is the center
	screenCoords = glm::vec2(screenCoords.x - screenWidth / 2, -(screenCoords.y - screenHeight / 2));
	// ������ ����ȭ | Scale the coordinates
	screenCoords /= scale;
	// ī�޶� �������� ��ȯ | Translate with the camera position
	screenCoords -= position;

	return screenCoords;
}
