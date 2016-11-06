#include "FractalSprite.h"


/* - 클래스 시작시 호출되는 함수 | Function called when the class start - */
Fractal::Sprite2D::Sprite2D() // 스프라이트 함수를 시작할때
{
	vboID_2D = 0; // 항상 0으로 버퍼 ID를 초기화 해둘 것 | Always initialize your buffer IDs to 0
}


/* - 클래스 종료시 호출되는 함수 | Function called when the class end - */
Fractal::Sprite2D::~Sprite2D() // 스프라이트 함수를 끝낼때
{
	// 작업이 끝나면 항상 버퍼를 삭제하는 것을 잊지 말 것 | Always remember to delete your buffers when you are done
	if (vboID_2D != 0) // _vboID 에 무언가 저장되 있는 경우
	{
		glDeleteBuffers(1, &vboID_2D); // GPU VRAM에 저장된 _vboID 버퍼 지우기
	}
}


/* - vbo 버퍼 만들기 및 스프라이트 위치와 크기 설정(x 좌표 위치, y 좌표 위치, 도형 넖이, 도형 높이) | Make vbo buffer and set the sprite position and scale - */
void Fractal::Sprite2D::Init(float x, float y, float width, float height, std::string texturePath)
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;
	_texture = ResourceManager::GetTexture(texturePath);

	if (vboID_2D == 0) // _vboID 에 아무것도 없으면
	{
		glGenBuffers(1, &vboID_2D); // GPU에 vbo 버퍼 생성 요청 (1 사이즈의 vboID)
	}

	Vertex vertexData[6]; // 버텍스 저장용 vbo 버퍼 (6개 버텍스로 이루어진 사각형 (삼각형 2개))

	// 첫번째 삼각형
	vertexData[0].SetPosition(x, y + height);
	vertexData[0].SetUV(0.0f, 1.0f);

	vertexData[1].SetPosition(x, y);
	vertexData[1].SetUV(0.0f, 0.0f);

	vertexData[2].SetPosition(x + width, y);
	vertexData[2].SetUV(1.0f, 0.0f);

	// 두번째 삼각형
	vertexData[3].SetPosition(x + width, y);
	vertexData[3].SetUV(1.0f, 0.0f);

	vertexData[4].SetPosition(x + width, y + height);
	vertexData[4].SetUV(1.0f, 1.0f);

	vertexData[5].SetPosition(x, y + height);
	vertexData[5].SetUV(0.0f, 1.0f);

	// 버텍스별 색 입히기 (셰이더 컬러 사용시 무시)
	for (int i = 0; i < 6; i++)
	{
		vertexData[i].SetColor(100, 100, 100, 255); //기본 컬러 입력(r, g, b, a)
	}

	// 버퍼 대입 (버텍스 버퍼 넣기)
	glBindBuffer(GL_ARRAY_BUFFER, vboID_2D);

	// 버퍼 데이터를 GPU에 업로드
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// 버퍼 대입 (버텍스 버퍼 비우기)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/* - 스프라이트 그리는 함수 | Draw a sprite - */
void Fractal::Sprite2D::Draw()
{
	// 텍스쳐 대입
	glBindTexture(GL_TEXTURE_2D, _texture.id);

	// 버퍼 대입 (버텍스 이름 넣기)
	glBindBuffer(GL_ARRAY_BUFFER, vboID_2D);

	// 버텍스 속성 배열을 활성화
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// 버텍스 위치 데이터의 배열을 정의 | This is the position attribute pointer
	glVertexAttribPointer(0, 2/*3D는 3*/, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	// 버텍스 색상 데이터의 배열을 정의 | This is the color attribute pointer
	glVertexAttribPointer(1, 4/*RGBA*/, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	// 버텍스 UV 데이터의 배열을 정의 | This is the UV attribute pointer
	glVertexAttribPointer(2, 2/*UV*/, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	// 그리기 방식
	glDrawArrays(GL_TRIANGLES, 0, 6/*버텍스 수*/);

	// 버텍스 속성 배열을 비활성화
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// 버퍼 대입 (버텍스 이름 비우기)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
