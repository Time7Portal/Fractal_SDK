#pragma once
#include <GLM\glm.hpp>
#include "FractalDebug.h"
#include "FractalSpriteBatch.h"
#include "FractalGLTexture.h"
#include "FractalResourceManager.h"


namespace Fractal
{
	class Bullet
	{
	public:
		Bullet(glm::vec2 newPosition, glm::vec2 newDirection, float newSpeed, int newLifeTime); // Ŭ���� ���۽� �Ѿ��� �ʱ�ȭ �ϴ� �Լ� (�ʱ���ġ, ����, �ӷ�, �����Ⱓ) | Initialize bullet system when the class start (Postion, Direction, Speed, Lifetime)
		~Bullet(); // Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end

		void Draw(Fractal::SpriteBatch& spriteBatch); // �Ѿ��� �׸��� �Լ� | Draw the bullet (Sprite batch)
		bool Update(); // �Ѿ� ���¸� ������Ʈ �ϴ� �Լ� (�����Ⱓ�� ������ TRUE ��ȯ) | Update the bullet (If lifetime is finished, return TRUE)

	private:
		float speed; // �Ѿ� �ӷ� | Bullet speed
		glm::vec2 direction; // �Ѿ� ���� | Bullet direction
		glm::vec2 position; // �Ѿ� ��ġ | Bullet position
		int lifeTime; // �Ѿ� ���� �ð� | Bullet lifetime
	};
}
