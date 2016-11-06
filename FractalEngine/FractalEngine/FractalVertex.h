#pragma once
#include <GL/glew.h>


namespace Fractal
{
	struct Vertex // 버텍스 속성 저장용 구조체
	{
		struct Position
		{
			float x; // 버텍스 위치의 x좌표
			float y; // 버텍스 위치의 y좌표
		};

		struct UV
		{
			float u; // UV 좌표의 u
			float v; // UV 좌표의 v
		};

		struct ColorRGBA8
		{
			ColorRGBA8() : r(255), g(255), b(255), a(255) { } // 버텍스 색 받기 (값이 생략되면 기본 버텍스 색인 흰색 지정)
			ColorRGBA8(GLubyte newR, GLubyte newG, GLubyte newB, GLubyte newA) : r(newR), g(newG), b(newB), a(newA) { } // 버텍스 색 받기

			GLubyte r; // 버텍스 Red 강도
			GLubyte g; // 버텍스 Green 강도
			GLubyte b; // 버텍스 Blue 강도
			GLubyte a; // 버텍스 Alpha 강도
		};

		// 구조체 접근을 쉽게 하기 위한 함수들
		void SetPosition(float x, float y)
		{
			position.x = x;
			position.y = y;
		}

		void SetUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}

		void SetColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		Position position;
		UV uv; // UV 텍스쳐 좌표 UV | texture coordinates
		ColorRGBA8 color;
	};
}