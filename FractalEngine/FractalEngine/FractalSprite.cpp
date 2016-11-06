#include "FractalSprite.h"


/* - Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start - */
Fractal::Sprite2D::Sprite2D() // ��������Ʈ �Լ��� �����Ҷ�
{
	vboID_2D = 0; // �׻� 0���� ���� ID�� �ʱ�ȭ �ص� �� | Always initialize your buffer IDs to 0
}


/* - Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end - */
Fractal::Sprite2D::~Sprite2D() // ��������Ʈ �Լ��� ������
{
	// �۾��� ������ �׻� ���۸� �����ϴ� ���� ���� �� �� | Always remember to delete your buffers when you are done
	if (vboID_2D != 0) // _vboID �� ���� ����� �ִ� ���
	{
		glDeleteBuffers(1, &vboID_2D); // GPU VRAM�� ����� _vboID ���� �����
	}
}


/* - vbo ���� ����� �� ��������Ʈ ��ġ�� ũ�� ����(x ��ǥ ��ġ, y ��ǥ ��ġ, ���� ����, ���� ����) | Make vbo buffer and set the sprite position and scale - */
void Fractal::Sprite2D::Init(float x, float y, float width, float height, std::string texturePath)
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;
	_texture = ResourceManager::GetTexture(texturePath);

	if (vboID_2D == 0) // _vboID �� �ƹ��͵� ������
	{
		glGenBuffers(1, &vboID_2D); // GPU�� vbo ���� ���� ��û (1 �������� vboID)
	}

	Vertex vertexData[6]; // ���ؽ� ����� vbo ���� (6�� ���ؽ��� �̷���� �簢�� (�ﰢ�� 2��))

	// ù��° �ﰢ��
	vertexData[0].SetPosition(x, y + height);
	vertexData[0].SetUV(0.0f, 1.0f);

	vertexData[1].SetPosition(x, y);
	vertexData[1].SetUV(0.0f, 0.0f);

	vertexData[2].SetPosition(x + width, y);
	vertexData[2].SetUV(1.0f, 0.0f);

	// �ι�° �ﰢ��
	vertexData[3].SetPosition(x + width, y);
	vertexData[3].SetUV(1.0f, 0.0f);

	vertexData[4].SetPosition(x + width, y + height);
	vertexData[4].SetUV(1.0f, 1.0f);

	vertexData[5].SetPosition(x, y + height);
	vertexData[5].SetUV(0.0f, 1.0f);

	// ���ؽ��� �� ������ (���̴� �÷� ���� ����)
	for (int i = 0; i < 6; i++)
	{
		vertexData[i].SetColor(100, 100, 100, 255); //�⺻ �÷� �Է�(r, g, b, a)
	}

	// ���� ���� (���ؽ� ���� �ֱ�)
	glBindBuffer(GL_ARRAY_BUFFER, vboID_2D);

	// ���� �����͸� GPU�� ���ε�
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// ���� ���� (���ؽ� ���� ����)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/* - ��������Ʈ �׸��� �Լ� | Draw a sprite - */
void Fractal::Sprite2D::Draw()
{
	// �ؽ��� ����
	glBindTexture(GL_TEXTURE_2D, _texture.id);

	// ���� ���� (���ؽ� �̸� �ֱ�)
	glBindBuffer(GL_ARRAY_BUFFER, vboID_2D);

	// ���ؽ� �Ӽ� �迭�� Ȱ��ȭ
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// ���ؽ� ��ġ �������� �迭�� ���� | This is the position attribute pointer
	glVertexAttribPointer(0, 2/*3D�� 3*/, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	// ���ؽ� ���� �������� �迭�� ���� | This is the color attribute pointer
	glVertexAttribPointer(1, 4/*RGBA*/, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	// ���ؽ� UV �������� �迭�� ���� | This is the UV attribute pointer
	glVertexAttribPointer(2, 2/*UV*/, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	// �׸��� ���
	glDrawArrays(GL_TRIANGLES, 0, 6/*���ؽ� ��*/);

	// ���ؽ� �Ӽ� �迭�� ��Ȱ��ȭ
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// ���� ���� (���ؽ� �̸� ����)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
