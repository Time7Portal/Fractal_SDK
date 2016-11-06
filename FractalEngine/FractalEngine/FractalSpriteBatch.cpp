#include "FractalSpriteBatch.h"


/* - Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start - */
Fractal::Glyph::Glyph()
{
}


/* - �۸����� ����� �Լ� (�簢�� ��ġ �� ũ��(xyzw), �ؽ��� UV ��ǥ �� ũ��(xyzw), �ؽ��� ID, ����, ��) | Make the Glyph (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color) - */
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


/* - ���� ������ �߰��� �۸����� ����� �Լ� (�簢�� ��ġ �� ũ��(xyzw), �ؽ��� UV ��ǥ �� ũ��(xyzw), �ؽ��� ID, ����, ��, ����) | (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color, angle) - */
Fractal::Glyph::Glyph(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint newTexture, float newDepth, const Vertex::ColorRGBA8& newColor, float newAngle)
{
	// �۸����� ���� ��ġ ����
	glm::vec2 halfDimension(destRectangle.z / 2.0f, destRectangle.w / 2.0f);
	glm::vec2 centerToTopLeft(-halfDimension.x, halfDimension.y);
	glm::vec2 centerToTopRight(halfDimension.x, halfDimension.y);
	glm::vec2 centerToBottomLeft(-halfDimension.x, -halfDimension.y);
	glm::vec2 centerToBottomRight(halfDimension.x, -halfDimension.y);

	// ȸ������ ���� ����Ʈ ��ġ �̵�
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


/* - Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end - */
Fractal::Glyph::~Glyph()
{
}


/* - ���ؽ� ����Ʈ�� ȸ�� ��Ű�� �Լ� | Rotate the vertex point - */
glm::vec2 Fractal::Glyph::RotatePoint(glm::vec2 position, float angle)
{
	glm::vec2 newVertexPosition; // ȸ�� ������ ���� �̵��� ���ؽ� ��ġ�� ����
	newVertexPosition.x = (position.x * cos(angle)) - (position.y * sin(angle));
	newVertexPosition.y = (position.x * sin(angle)) + (position.y * cos(angle));

	return newVertexPosition;
}


/* - Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start - */
Fractal::SpriteBatch::SpriteBatch()
{
	_vbo = 0;
	_vao = 0;
}


/* - Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end - */
Fractal::SpriteBatch::~SpriteBatch()
{
}


/* - ��ġ�� �����ϱ� ���� �ʱ�ȭ �ϴ� �Լ� | Initialize to generate Batch - */
void Fractal::SpriteBatch::Init()
{
	CreateVertexArray();
}


/* - ��������Ʈ �Է��� �غ��Ű�� �Լ� (���Ĺ��) | Prepare to make sprite - */
void Fractal::SpriteBatch::Start(GlyphSortType sortType) // sortType = GlyphSortType::TEXTURE
{
	_sortType = sortType;

	// �޸� ���� ���� | Don't leak the RAM
	_renderBatches.clear();
	_glyphs.clear();
}


/* - ��ġ�� �Է¹޾� �����ϴ� �Լ� (�簢�� ��ġ �� ũ��(xyzw), �ؽ��� UV ��ǥ �� ũ��(xyzw), �ؽ��� ID, ����, ��) | Make the batch square (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color) - */
void Fractal::SpriteBatch::Draw(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const Vertex::ColorRGBA8& color)
{
	_glyphs.emplace_back(destRectangle, uvRectangle, texture, depth, color);
}


/* - ���� ������ �߰��� ��ġ�� �Է¹޾� �����ϴ� �Լ� (�簢�� ��ġ �� ũ��(xyzw), �ؽ��� UV ��ǥ �� ũ��(xyzw), �ؽ��� ID, ����, ��, ����) | Make the batch square (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color, angle) - */
void Fractal::SpriteBatch::Draw(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const Vertex::ColorRGBA8& color, float angle)
{
	_glyphs.emplace_back(destRectangle, uvRectangle, texture, depth, color, angle);
}


/* - ���� ������ �߰��� ��ġ�� �Է¹޾� �����ϴ� �Լ� (�簢�� ��ġ �� ũ��(xyzw), �ؽ��� UV ��ǥ �� ũ��(xyzw), �ؽ��� ID, ����, ��, ����) | Make the batch square (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color, direction) - */
void Fractal::SpriteBatch::Draw(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const Vertex::ColorRGBA8& color, glm::vec2& direction)
{
	// ���� ������ �̿��� ���� ���
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


/* - ��������Ʈ �Է��� �����Ű�� �Լ� | End to make sprite - */
void Fractal::SpriteBatch::End()
{
	// ���� ������ ���� ��� ������ �ʱ�ȭ | Set up all pointers for fast sorting
	_glyphPointers.resize(_glyphs.size());
	for (int i = 0; i < _glyphs.size(); i++)
	{
		_glyphPointers[i] = &_glyphs[i];
	}

	SortGlyphs();
	CreateRenderBatches();
}


/* - �ϼ��� ��������Ʈ�� ȭ�鿡 ����ϴ� �Լ� | Draw the complete sprite on the screen - */
void Fractal::SpriteBatch::RenderBatch()
{
	// VAO ���� | Bind our VAO. This sets up the OpenGL state we need, including the vertex attribute pointers and it binds the VBO
	glBindVertexArray(_vao);

	for (int i = 0; i < _renderBatches.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
		glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
	}

	glBindVertexArray(0);
}


/* - ����� ��ġ�� ����� �Լ� | Create the batch to render - */
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

	int offset = 0; // ���� ������ �� | Current offset
	int cv = 0; // ���� ���ؽ� ��ȣ | Current Vertex
	
	// ù��° �۸��� �����ϱ� | Add the first batch
	_renderBatches.emplace_back(0, 6, _glyphPointers[0]->texture);
	vertices[cv++] = _glyphPointers[0]->topLeft;
	vertices[cv++] = _glyphPointers[0]->bottomLeft;
	vertices[cv++] = _glyphPointers[0]->bottomRight;
	vertices[cv++] = _glyphPointers[0]->bottomRight;
	vertices[cv++] = _glyphPointers[0]->topRight;
	vertices[cv++] = _glyphPointers[0]->topLeft;
	offset += 6; // �۸����� �ﰢ�� 2��, �� 6���� ������ �̷�� �� | Glyphs are made by two triangles, that consists of six points

	// ������ ��� �۸��� �����ϱ� | Add all the rest of the glyphs
	for (int cg = 1; cg < _glyphPointers.size(); cg++) // cg = ���� �۸��� ��ȣ | cg = Current glyph
	{
		// ��ȿ�� ��ġ�� ����� �ִ� ��� | Check if this glyph can be part of the current batch
		if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture)
		{
			_renderBatches.emplace_back(offset, 6, _glyphPointers[cg]->texture); // ��ġ ����� | Make a new batch
		}
		else
		{
			_renderBatches.back().numVertices += 6;  // ��ȿ���� ������ �ش� ��ġ�� ��� ���ؽ� ��ȣ�� �ǳʶ� | If it's part of the current batch, just increase numVertices
		}
		vertices[cv++] = _glyphPointers[cg]->topLeft;
		vertices[cv++] = _glyphPointers[cg]->bottomLeft;
		vertices[cv++] = _glyphPointers[cg]->bottomRight;
		vertices[cv++] = _glyphPointers[cg]->bottomRight;
		vertices[cv++] = _glyphPointers[cg]->topRight;
		vertices[cv++] = _glyphPointers[cg]->topLeft;
		offset += 6;
	}

	// ���� ���� (���ؽ� ���� �ֱ�) | Bind the VBO buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	// ���ɸ� ���� ���۸� ����ȭ | Orphan the buffer for speed
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	// �����͸� GPU�� ���� | Upload data to the GPU
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

	// ���� ���� (���ؽ� ���� ����) | Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/* - ���ؽ� �迭�� ���۸� ����� �Լ� | Create the VAO, VBO - */
void Fractal::SpriteBatch::CreateVertexArray()
{
	// ������� VAO �� ������ ���� �Ҵ�ޱ� | Generate the VAO if it isn't already generated
	if (_vao == 0)
	{
		glGenVertexArrays(1, &_vao);
	}

	glBindVertexArray(_vao); // Bind the VAO. All subsequent opengl calls will modify it's state.

	// ������� VBO �� ������ ���� �Ҵ�ޱ� | Generate the VBO if it isn't already generated
	if (_vbo == 0)
	{
		glGenBuffers(1, &_vbo);
	}

	// ���� ���� (���ؽ� ���� �̸�)
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	// ���ؽ� �Ӽ� �迭�� Ȱ��ȭ | Tell GPU what attribute arrays we need
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// ���ؽ� ��ġ �������� �迭�� ���� | This is the position attribute pointer
	glVertexAttribPointer(0, 2/*3D�� 3*/, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	// ���ؽ� ���� �������� �迭�� ���� | This is the color attribute pointer
	glVertexAttribPointer(1, 4/*RGBA*/, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	// ���ؽ� UV �������� �迭�� ���� | This is the UV attribute pointer
	glVertexAttribPointer(2, 2/*UV*/, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	// VAO ���� ����
	glBindVertexArray(0);
}


/* - �۸����� �����ϴ� �Լ� | Sort the Glyphs - */
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


/* - ������ ���Ĺ�Ŀ� ���� ��ġ�� �����ϴ� �Լ��� | Sorting the arrangement according to the specified alignment - */
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
