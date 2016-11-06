#include "FractalBullet.h"


/* - 클래스 시작시 총알을 초기화 하는 함수 | Initialize bullet system when the class start (Postion, Direction, Speed, Lifetime) - */
Fractal::Bullet::Bullet(glm::vec2 newPosition, glm::vec2 newDirection, float newSpeed, int newLifeTime)
{
	position = newPosition;
	direction = newDirection;
	speed = newSpeed;
	lifeTime = newLifeTime;
}


/* - 클래스 종료시 호출되는 함수 | Function called when the class end - */
Fractal::Bullet::~Bullet()
{
}


/* - 총알을 그리는 함수 | Draw the bullet (Sprite batch) - */
void Fractal::Bullet::Draw(Fractal::SpriteBatch& spriteBatch)
{
	// 총알 이미지 셋팅
	static std::string programPath = Fractal::Debug::GetInstance()->GetProgramPath(); // 실행중인 프로그램 경로를 가져오기 | Get program path
	static Fractal::GLTexture texture = Fractal::ResourceManager::GetTexture(programPath + "\\Texture\\Creepy_Ham.png");
	Fractal::Vertex::ColorRGBA8 color; // 버텍스 컬러
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f); // UV 사각형 (UV 좌표)
	glm::vec4 posAndSize = glm::vec4(position.x, position.y, 30, 30); // 위치 와 크기값을 vec4로 전환

	// 그리기
	spriteBatch.Draw(posAndSize, uv, texture.id, 0.0f, color);
}


/* - 총알 상태를 업데이트 하는 함수 (생존기간이 끝나면 TRUE 반환) | Update the bullet (If lifetime is finished, return TRUE) - */
bool Fractal::Bullet::Update()
{
	position += direction * speed; // 방향과 속력을 곱해 다음 총알의 위치를 계산
	lifeTime--; // 생존 시간 감소

	// 생존 기간이 끝난 경우 TRUE 반환
	if (lifeTime == 0)
	{
		return true;
	}

	// 생존 기간일 경우 FALSE 반환
	return false;
}
