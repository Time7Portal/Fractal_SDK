#pragma once
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>


namespace Fractal
{
	class Camera2D
	{
	public:
		Camera2D(); // Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start
		~Camera2D(); // Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end

		void Init(int newScreenWidth, int newScreenHeight); // ī�޶� �ʱ�ȭ �ϴ� �Լ� | Sets up the Orthographic matrix and Screen dimensions

		void Update(); // ī�޶� ��ǥ�� ������Ʈ �ϴ� �Լ� | Updates the camera matrix if needed

		glm::vec2 ConvertScreenToWorld(glm::vec2 screenCoords); // ��ũ�� ��ǥ�� ���� ��ǥ�� ��ȯ�ϴ� �Լ� | Convert the Screen coordinate to World

		void SetPosition(const glm::vec2& newPosition); // ī�޶� ��ġ�� �����ϴ� �Լ� | Set Camera Position

		void SetScale(float newScale); // ī�޶� ���� �����ϴ� �Լ� | Set Camera Scale 

		// ī�޶� �Ӽ����� ��ȯ�ϴ� �Լ��� | Get Camera attributes
		glm::vec2 GetPosition(){ return position; }
		float GetScale(){ return scale; }
		glm::mat4 GetCameraMatrix() { return cameraMatrix; }

	private:
		int screenWidth, screenHeight; // ��ũ�� ����, ���� | Screen size
		bool needsMatrixUpdate; // ī�޶� �Ӽ��� ���Ͽ� ������Ʈ�� �ʿ����� | Needs camera Matrix update
		float scale; // ī�޶� �� ũ�� | Camera scale
		glm::vec2 position; // ī�޶� ��ġ | Camera position
		glm::mat4 orthoMatrix; // ī�޶� ������ | Camera projection mode
		glm::mat4 cameraMatrix; // ī�޶� ��ǥ�� | Camera matrix
	};
}
