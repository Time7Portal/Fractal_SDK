#pragma once
#include <algorithm>
#include <vector>
#include <GL\glew.h>
#include <GLM\glm.hpp>
#include "FractalVertex.h"


namespace Fractal
{
	enum class GlyphSortType // �۸��� ���� Ÿ�� ������ ���� | Glyph sort type enum class
	{
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};


	class Glyph // �۸����� ���� �����ν� ����� �簢�� �ϳ��� ���� | Glyph is quad square unit
	{
	public:
		Glyph(); // Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start
		Glyph(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint newTexture, float newDepth, const Vertex::ColorRGBA8& newColor); // �۸����� ����� �Լ� (�簢�� ��ġ �� ũ��(xyzw), �ؽ��� UV ��ǥ �� ũ��(xyzw), �ؽ��� ID, ����, ��) | Make the Glyph (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color)
		Glyph(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint newTexture, float newDepth, const Vertex::ColorRGBA8& newColor, float newAngle); // ���� ������ �߰��� �۸����� ����� �Լ� (�簢�� ��ġ �� ũ��(xyzw), �ؽ��� UV ��ǥ �� ũ��(xyzw), �ؽ��� ID, ����, ��, ����) | (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color, angle)
		~Glyph(); // Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end

		GLuint texture; // �ؽ��� ��ȣ | Texture ID
		float depth; // �۸��� ���� | Glyph depth

		Vertex topLeft; // �� ��ǥ - �� ���� | Point coordinates - top left
		Vertex topRight; // �� ��ǥ - �� ������ | Point coordinates - top right
		Vertex bottomLeft; // �� ��ǥ - �Ʒ� ���� | Point coordinates - bottom left
		Vertex bottomRight; // �� ��ǥ - �Ʒ� ������ | Point coordinates - bottom right

	private:
		glm::vec2 RotatePoint(glm::vec2 position, float angle); // ���ؽ� ����Ʈ�� ȸ�� ��Ű�� �Լ� | Rotate the vertex point
	};


	class BatchRender
	{
	public:
		GLuint offset;
		GLuint numVertices;
		GLuint texture;

		BatchRender(GLuint newOffset, GLuint newNumVertices, GLuint newTexture) : offset(newOffset), numVertices(newNumVertices), texture(newTexture) { }
	};


	class SpriteBatch
	{
	public:
		SpriteBatch(); // Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start
		~SpriteBatch(); // Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end

		void Init(); // ��ġ�� �����ϱ� ���� �ʱ�ȭ �ϴ� �Լ� | Initialize to generate Batch

		void Start(GlyphSortType sortType = GlyphSortType::TEXTURE); // ��������Ʈ �Է��� �غ��Ű�� �Լ� (���Ĺ��) | Prepare to make sprite

		void Draw(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const Vertex::ColorRGBA8& color); // ��ġ�� �Է¹޾� �����ϴ� �Լ� (�簢�� ��ġ �� ũ��(xyzw), �ؽ��� UV ��ǥ �� ũ��(xyzw), �ؽ��� ID, ����, ��) | Make the batch square (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color)
		void Draw(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const Vertex::ColorRGBA8& color, float angle); // ���� ������ �߰��� ��ġ�� �Է¹޾� �����ϴ� �Լ� (�簢�� ��ġ �� ũ��(xyzw), �ؽ��� UV ��ǥ �� ũ��(xyzw), �ؽ��� ID, ����, ��, ����) | Make the batch square (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color, angle)
		void Draw(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const Vertex::ColorRGBA8& color, glm::vec2& direction); // ���� ������ �߰��� ��ġ�� �Է¹޾� �����ϴ� �Լ� (�簢�� ��ġ �� ũ��(xyzw), �ؽ��� UV ��ǥ �� ũ��(xyzw), �ؽ��� ID, ����, ��, ����) | Make the batch square (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color, direction)

		void End(); // ��������Ʈ �Է��� �����Ű�� �Լ� | End to make sprite

		void RenderBatch(); // �ϼ��� ��������Ʈ�� ȭ�鿡 ����ϴ� �Լ� | Draw the complete sprite on the screen

	private:
		void CreateRenderBatches(); // ����� ��ġ�� ����� �Լ� | Create the batch to render
		void CreateVertexArray(); // ���ؽ� �迭�� ���۸� ����� �Լ� | Create the VAO, VBO
		void SortGlyphs(); // �۸����� �����ϴ� �Լ� | Sort the Glyphs

		// ������ ���Ĺ�Ŀ� ���� ��ġ�� �����ϴ� �Լ��� | Sorting the arrangement according to the specified alignment
		static bool CompareFrontToBack(Glyph* a, Glyph* b);
		static bool CompareBackToFront(Glyph* a, Glyph* b);
		static bool CompareTexture(Glyph* a, Glyph* b);

		GLuint _vbo; // VBO ��ȣ | VBO ID
		GLuint _vao; // VAO ��ȣ | VAO ID

		GlyphSortType _sortType; // ��ġ ���� ��� | Batch Sort Type

		std::vector<Glyph*> _glyphPointers; // �۸��� ���Ŀ� ������ | Glyph ID pointer for sorting glyphs
		std::vector<Glyph> _glyphs; // �۸��� ���� | Glyph ID
		std::vector<BatchRender> _renderBatches; // ��ġ ���� ���� | Batch render ID for render batches
	};
}
