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
		Bullet(glm::vec2 newPosition, glm::vec2 newDirection, float newSpeed, int newLifeTime); // 클래스 시작시 총알을 초기화 하는 함수 (초기위치, 방향, 속력, 생존기간) | Initialize bullet system when the class start (Postion, Direction, Speed, Lifetime)
		~Bullet(); // 클래스 종료시 호출되는 함수 | Function called when the class end

		void Draw(Fractal::SpriteBatch& spriteBatch); // 총알을 그리는 함수 | Draw the bullet (Sprite batch)
		bool Update(); // 총알 상태를 업데이트 하는 함수 (생존기간이 끝나면 TRUE 반환) | Update the bullet (If lifetime is finished, return TRUE)

	private:
		float speed; // 총알 속력 | Bullet speed
		glm::vec2 direction; // 총알 방향 | Bullet direction
		glm::vec2 position; // 총알 위치 | Bullet position
		int lifeTime; // 총알 생존 시간 | Bullet lifetime
	};
}
