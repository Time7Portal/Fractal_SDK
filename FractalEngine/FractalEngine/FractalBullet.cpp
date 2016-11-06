#include "FractalBullet.h"


/* - Ŭ���� ���۽� �Ѿ��� �ʱ�ȭ �ϴ� �Լ� | Initialize bullet system when the class start (Postion, Direction, Speed, Lifetime) - */
Fractal::Bullet::Bullet(glm::vec2 newPosition, glm::vec2 newDirection, float newSpeed, int newLifeTime)
{
	position = newPosition;
	direction = newDirection;
	speed = newSpeed;
	lifeTime = newLifeTime;
}


/* - Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end - */
Fractal::Bullet::~Bullet()
{
}


/* - �Ѿ��� �׸��� �Լ� | Draw the bullet (Sprite batch) - */
void Fractal::Bullet::Draw(Fractal::SpriteBatch& spriteBatch)
{
	// �Ѿ� �̹��� ����
	static std::string programPath = Fractal::Debug::GetInstance()->GetProgramPath(); // �������� ���α׷� ��θ� �������� | Get program path
	static Fractal::GLTexture texture = Fractal::ResourceManager::GetTexture(programPath + "\\Texture\\Creepy_Ham.png");
	Fractal::Vertex::ColorRGBA8 color; // ���ؽ� �÷�
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f); // UV �簢�� (UV ��ǥ)
	glm::vec4 posAndSize = glm::vec4(position.x, position.y, 30, 30); // ��ġ �� ũ�Ⱚ�� vec4�� ��ȯ

	// �׸���
	spriteBatch.Draw(posAndSize, uv, texture.id, 0.0f, color);
}


/* - �Ѿ� ���¸� ������Ʈ �ϴ� �Լ� (�����Ⱓ�� ������ TRUE ��ȯ) | Update the bullet (If lifetime is finished, return TRUE) - */
bool Fractal::Bullet::Update()
{
	position += direction * speed; // ����� �ӷ��� ���� ���� �Ѿ��� ��ġ�� ���
	lifeTime--; // ���� �ð� ����

	// ���� �Ⱓ�� ���� ��� TRUE ��ȯ
	if (lifeTime == 0)
	{
		return true;
	}

	// ���� �Ⱓ�� ��� FALSE ��ȯ
	return false;
}
