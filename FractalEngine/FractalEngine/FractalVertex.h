#pragma once
#include <GL/glew.h>


namespace Fractal
{
	struct Vertex // ���ؽ� �Ӽ� ����� ����ü
	{
		struct Position
		{
			float x; // ���ؽ� ��ġ�� x��ǥ
			float y; // ���ؽ� ��ġ�� y��ǥ
		};

		struct UV
		{
			float u; // UV ��ǥ�� u
			float v; // UV ��ǥ�� v
		};

		struct ColorRGBA8
		{
			ColorRGBA8() : r(255), g(255), b(255), a(255) { } // ���ؽ� �� �ޱ� (���� �����Ǹ� �⺻ ���ؽ� ���� ��� ����)
			ColorRGBA8(GLubyte newR, GLubyte newG, GLubyte newB, GLubyte newA) : r(newR), g(newG), b(newB), a(newA) { } // ���ؽ� �� �ޱ�

			GLubyte r; // ���ؽ� Red ����
			GLubyte g; // ���ؽ� Green ����
			GLubyte b; // ���ؽ� Blue ����
			GLubyte a; // ���ؽ� Alpha ����
		};

		// ����ü ������ ���� �ϱ� ���� �Լ���
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
		UV uv; // UV �ؽ��� ��ǥ UV | texture coordinates
		ColorRGBA8 color;
	};
}