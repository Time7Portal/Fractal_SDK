#include "FractalSpriteBatch.h"


/* - 클래스 시작시 호출되는 함수 | Function called when the class start - */
Fractal::Glyph::Glyph()
{
}


/* - 글리프를 만드는 함수 (사각형 위치 및 크기(xyzw), 텍스쳐 UV 좌표 및 크기(xyzw), 텍스쳐 ID, 깊이, 색) | Make the Glyph (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color) - */
Fractal::Glyph::Glyph(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint newTexture, float newDepth, const Vertex::ColorRGBA8& newColor)
{
	texture = newTexture;
	depth = newDepth;

	topLeft.color = newColor;
	topLeft.SetPosition(destRectangle.x, destRectangle.y + destRectangle.w);
	topLeft.SetUV(uvRectangle.x, uvRectangle.y + uvRectangle.w);

	topRight.color = newColor;
	topRight.SetPosition(destRectangle.x + destRectangle.z, destRectangle.y + destRectangle.w);
	topRight.SetUV(uvRectangle.x + uvRectangle.z, uvRectangle.y + uvRectangle.w);

	bottomLeft.color = newColor;
	bottomLeft.SetPosition(destRectangle.x, destRectangle.y);
	bottomLeft.SetUV(uvRectangle.x, uvRectangle.y);

	bottomRight.color = newColor;
	bottomRight.SetPosition(destRectangle.x + destRectangle.z, destRectangle.y);
	bottomRight.SetUV(uvRectangle.x + uvRectangle.z, uvRectangle.y);
}


/* - 각도 정보가 추가된 글리프를 만드는 함수 (사각형 위치 및 크기(xyzw), 텍스쳐 UV 좌표 및 크기(xyzw), 텍스쳐 ID, 깊이, 색, 각도) | (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color, angle) - */
Fractal::Glyph::Glyph(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint newTexture, float newDepth, const Vertex::ColorRGBA8& newColor, float newAngle)
{
	// 글리프의 센터 위치 생성
	glm::vec2 halfDimension(destRectangle.z / 2.0f, destRectangle.w / 2.0f);
	glm::vec2 centerToTopLeft(-halfDimension.x, halfDimension.y);
	glm::vec2 centerToTopRight(halfDimension.x, halfDimension.y);
	glm::vec2 centerToBottomLeft(-halfDimension.x, -halfDimension.y);
	glm::vec2 centerToBottomRight(halfDimension.x, -halfDimension.y);

	// 회전값에 따른 포인트 위치 이동
	centerToTopLeft = RotatePoint(centerToTopLeft, newAngle) + halfDimension;
	centerToTopRight = RotatePoint(centerToTopRight, newAngle) + halfDimension;
	centerToBottomLeft = RotatePoint(centerToBottomLeft, newAngle) + halfDimension;
	centerToBottomRight = RotatePoint(centerToBottomRight, newAngle) + halfDimension;
	
	texture = newTexture;
	depth = newDepth;

	topLeft.color = newColor;
	topLeft.SetPosition(destRectangle.x + centerToTopLeft.x, destRectangle.y + centerToTopLeft.y);
	topLeft.SetUV(uvRectangle.x, uvRectangle.y + uvRectangle.w);

	topRight.color = newColor;
	topRight.SetPosition(destRectangle.x + centerToTopRight.x, destRectangle.y + centerToTopRight.y);
	topRight.SetUV(uvRectangle.x + uvRectangle.z, uvRectangle.y + uvRectangle.w);

	bottomLeft.color = newColor;
	bottomLeft.SetPosition(destRectangle.x + centerToBottomLeft.x, destRectangle.y + centerToBottomLeft.y);
	bottomLeft.SetUV(uvRectangle.x, uvRectangle.y);

	bottomRight.color = newColor;
	bottomRight.SetPosition(destRectangle.x + centerToBottomRight.x, destRectangle.y + centerToBottomRight.y);
	bottomRight.SetUV(uvRectangle.x + uvRectangle.z, uvRectangle.y);
}


/* - 클래스 종료시 호출되는 함수 | Function called when the class end - */
Fractal::Glyph::~Glyph()
{
}


/* - 버텍스 포인트를 회전 시키는 함수 | Rotate the vertex point - */
glm::vec2 Fractal::Glyph::RotatePoint(glm::vec2 position, float angle)
{
	glm::vec2 newVertexPosition; // 회전 각도에 따라 이동된 버텍스 위치를 생성
	newVertexPosition.x = (position.x * cos(angle)) - (position.y * sin(angle));
	newVertexPosition.y = (position.x * sin(angle)) + (position.y * cos(angle));

	return newVertexPosition;
}


/* - 클래스 시작시 호출되는 함수 | Function called when the class start - */
Fractal::SpriteBatch::SpriteBatch()
{
	_vbo = 0;
	_vao = 0;
}


/* - 클래스 종료시 호출되는 함수 | Function called when the class end - */
Fractal::SpriteBatch::~SpriteBatch()
{
}


/* - 배치를 생성하기 위해 초기화 하는 함수 | Initialize to generate Batch - */
void Fractal::SpriteBatch::Init()
{
	CreateVertexArray();
}


/* - 스프라이트 입력을 준비시키는 함수 (정렬방식) | Prepare to make sprite - */
void Fractal::SpriteBatch::Start(GlyphSortType sortType) // sortType = GlyphSortType::TEXTURE
{
	_sortType = sortType;

	// 메모리 누수 방지 | Don't leak the RAM
	_renderBatches.clear();
	_glyphs.clear();
}


/* - 배치를 입력받아 저장하는 함수 (사각형 위치 및 크기(xyzw), 텍스쳐 UV 좌표 및 크기(xyzw), 텍스쳐 ID, 깊이, 색) | Make the batch square (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color) - */
void Fractal::SpriteBatch::Draw(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const Vertex::ColorRGBA8& color)
{
	_glyphs.emplace_back(destRectangle, uvRectangle, texture, depth, color);
}


/* - 각도 정보가 추가된 배치를 입력받아 저장하는 함수 (사각형 위치 및 크기(xyzw), 텍스쳐 UV 좌표 및 크기(xyzw), 텍스쳐 ID, 깊이, 색, 각도) | Make the batch square (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color, angle) - */
void Fractal::SpriteBatch::Draw(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const Vertex::ColorRGBA8& color, float angle)
{
	_glyphs.emplace_back(destRectangle, uvRectangle, texture, depth, color, angle);
}


/* - 방향 정보가 추가된 배치를 입력받아 저장하는 함수 (사각형 위치 및 크기(xyzw), 텍스쳐 UV 좌표 및 크기(xyzw), 텍스쳐 ID, 깊이, 색, 방향) | Make the batch square (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color, direction) - */
void Fractal::SpriteBatch::Draw(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const Vertex::ColorRGBA8& color, glm::vec2& direction)
{
	// 방향 정보를 이용해 각도 계산
	const glm::vec2 right(1.0f, 0.0f);
	float angle;
	if (direction.y > 0.0f)
	{
		angle = acos(glm::dot(right, direction));
	}
	else
	{
		angle = -acos(glm::dot(right, direction));
	}

	_glyphs.emplace_back(destRectangle, uvRectangle, texture, depth, color, angle);
}


/* - 스프라이트 입력을 종료시키는 함수 | End to make sprite - */
void Fractal::SpriteBatch::End()
{
	// 빠른 정렬을 위해 모든 포인터 초기화 | Set up all pointers for fast sorting
	_glyphPointers.resize(_glyphs.size());
	for (int i = 0; i < _glyphs.size(); i++)
	{
		_glyphPointers[i] = &_glyphs[i];
	}

	SortGlyphs();
	CreateRenderBatches();
}


/* - 완성된 스프라이트를 화면에 출력하는 함수 | Draw the complete sprite on the screen - */
void Fractal::SpriteBatch::RenderBatch()
{
	// VAO 대입 | Bind our VAO. This sets up the OpenGL state we need, including the vertex attribute pointers and it binds the VBO
	glBindVertexArray(_vao);

	for (int i = 0; i < _renderBatches.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
		glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
	}

	glBindVertexArray(0);
}


/* - 출력할 배치를 만드는 함수 | Create the batch to render - */
void Fractal::SpriteBatch::CreateRenderBatches()
{
	// This will store all the vertices that we need to upload
	std::vector <Vertex> vertices;
	// Resize the buffer to the exact size we need so we can treat like an array
	vertices.resize(_glyphPointers.size() * 6);

	if (_glyphPointers.empty())
	{
		return;
	}

	int offset = 0; // 현재 오프셋 값 | Current offset
	int cv = 0; // 현재 버텍스 번호 | Current Vertex
	
	// 첫번째 글리프 생성하기 | Add the first batch
	_renderBatches.emplace_back(0, 6, _glyphPointers[0]->texture);
	vertices[cv++] = _glyphPointers[0]->topLeft;
	vertices[cv++] = _glyphPointers[0]->bottomLeft;
	vertices[cv++] = _glyphPointers[0]->bottomRight;
	vertices[cv++] = _glyphPointers[0]->bottomRight;
	vertices[cv++] = _glyphPointers[0]->topRight;
	vertices[cv++] = _glyphPointers[0]->topLeft;
	offset += 6; // 글리프는 삼각형 2개, 즉 6개의 점으로 이루어 짐 | Glyphs are made by two triangles, that consists of six points

	// 나머지 모든 글리프 생성하기 | Add all the rest of the glyphs
	for (int cg = 1; cg < _glyphPointers.size(); cg++) // cg = 현재 글리프 번호 | cg = Current glyph
	{
		// 유효한 배치를 만들수 있는 경우 | Check if this glyph can be part of the current batch
		if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture)
		{
			_renderBatches.emplace_back(offset, 6, _glyphPointers[cg]->texture); // 배치 만들기 | Make a new batch
		}
		else
		{
			_renderBatches.back().numVertices += 6;  // 유효하지 않으면 해당 배치의 모든 버텍스 번호를 건너뜀 | If it's part of the current batch, just increase numVertices
		}
		vertices[cv++] = _glyphPointers[cg]->topLeft;
		vertices[cv++] = _glyphPointers[cg]->bottomLeft;
		vertices[cv++] = _glyphPointers[cg]->bottomRight;
		vertices[cv++] = _glyphPointers[cg]->bottomRight;
		vertices[cv++] = _glyphPointers[cg]->topRight;
		vertices[cv++] = _glyphPointers[cg]->topLeft;
		offset += 6;
	}

	// 버퍼 대입 (버텍스 버퍼 넣기) | Bind the VBO buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	// 성능를 위해 버퍼를 독립화 | Orphan the buffer for speed
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	// 데이터를 GPU로 전달 | Upload data to the GPU
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

	// 버퍼 비우기 (버텍스 버퍼 제거) | Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/* - 버텍스 배열과 버퍼를 만드는 함수 | Create the VAO, VBO - */
void Fractal::SpriteBatch::CreateVertexArray()
{
	// 만들어진 VAO 가 없으면 새로 할당받기 | Generate the VAO if it isn't already generated
	if (_vao == 0)
	{
		glGenVertexArrays(1, &_vao);
	}

	glBindVertexArray(_vao); // Bind the VAO. All subsequent opengl calls will modify it's state.

	// 만들어진 VBO 가 없으면 새로 할당받기 | Generate the VBO if it isn't already generated
	if (_vbo == 0)
	{
		glGenBuffers(1, &_vbo);
	}

	// 버퍼 대입 (버텍스 버퍼 이름)
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	// 버텍스 속성 배열을 활성화 | Tell GPU what attribute arrays we need
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// 버텍스 위치 데이터의 배열을 정의 | This is the position attribute pointer
	glVertexAttribPointer(0, 2/*3D는 3*/, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	// 버텍스 색상 데이터의 배열을 정의 | This is the color attribute pointer
	glVertexAttribPointer(1, 4/*RGBA*/, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	// 버텍스 UV 데이터의 배열을 정의 | This is the UV attribute pointer
	glVertexAttribPointer(2, 2/*UV*/, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	// VAO 버퍼 비우기
	glBindVertexArray(0);
}


/* - 글리프를 정렬하는 함수 | Sort the Glyphs - */
void Fractal::SpriteBatch::SortGlyphs()
{
	switch (_sortType)
	{
	case GlyphSortType::FRONT_TO_BACK:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), CompareFrontToBack);
		break;

	case GlyphSortType::BACK_TO_FRONT:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), CompareBackToFront);
		break;

	case GlyphSortType::TEXTURE:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), CompareTexture);
		break;
	}
}


/* - 지정한 정렬방식에 따라 배치를 정렬하는 함수들 | Sorting the arrangement according to the specified alignment - */
bool Fractal::SpriteBatch::CompareFrontToBack(Glyph* a, Glyph* b)
{
	return (a->depth < b->depth);
}

bool Fractal::SpriteBatch::CompareBackToFront(Glyph* a, Glyph* b)
{
	return (a->depth > b->depth);
}

bool Fractal::SpriteBatch::CompareTexture(Glyph* a, Glyph* b)
{
	return (a->texture < b->texture);
}
