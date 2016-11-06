#pragma once
#include <algorithm>
#include <vector>
#include <GL\glew.h>
#include <GLM\glm.hpp>
#include "FractalVertex.h"


namespace Fractal
{
	enum class GlyphSortType // 글리프 정렬 타입 열거형 정의 | Glyph sort type enum class
	{
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};


	class Glyph // 글리프는 쿼드 단위로써 출력할 사각형 하나를 정의 | Glyph is quad square unit
	{
	public:
		Glyph(); // 클래스 시작시 호출되는 함수 | Function called when the class start
		Glyph(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint newTexture, float newDepth, const Vertex::ColorRGBA8& newColor); // 글리프를 만드는 함수 (사각형 위치 및 크기(xyzw), 텍스쳐 UV 좌표 및 크기(xyzw), 텍스쳐 ID, 깊이, 색) | Make the Glyph (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color)
		Glyph(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint newTexture, float newDepth, const Vertex::ColorRGBA8& newColor, float newAngle); // 각도 정보가 추가된 글리프를 만드는 함수 (사각형 위치 및 크기(xyzw), 텍스쳐 UV 좌표 및 크기(xyzw), 텍스쳐 ID, 깊이, 색, 각도) | (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color, angle)
		~Glyph(); // 클래스 종료시 호출되는 함수 | Function called when the class end

		GLuint texture; // 텍스쳐 번호 | Texture ID
		float depth; // 글리프 깊이 | Glyph depth

		Vertex topLeft; // 점 좌표 - 위 왼쪽 | Point coordinates - top left
		Vertex topRight; // 점 좌표 - 위 오른쪽 | Point coordinates - top right
		Vertex bottomLeft; // 점 좌표 - 아래 왼쪽 | Point coordinates - bottom left
		Vertex bottomRight; // 점 좌표 - 아래 오른쪽 | Point coordinates - bottom right

	private:
		glm::vec2 RotatePoint(glm::vec2 position, float angle); // 버텍스 포인트를 회전 시키는 함수 | Rotate the vertex point
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
		SpriteBatch(); // 클래스 시작시 호출되는 함수 | Function called when the class start
		~SpriteBatch(); // 클래스 종료시 호출되는 함수 | Function called when the class end

		void Init(); // 배치를 생성하기 위해 초기화 하는 함수 | Initialize to generate Batch

		void Start(GlyphSortType sortType = GlyphSortType::TEXTURE); // 스프라이트 입력을 준비시키는 함수 (정렬방식) | Prepare to make sprite

		void Draw(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const Vertex::ColorRGBA8& color); // 배치를 입력받아 저장하는 함수 (사각형 위치 및 크기(xyzw), 텍스쳐 UV 좌표 및 크기(xyzw), 텍스쳐 ID, 깊이, 색) | Make the batch square (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color)
		void Draw(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const Vertex::ColorRGBA8& color, float angle); // 각도 정보가 추가된 배치를 입력받아 저장하는 함수 (사각형 위치 및 크기(xyzw), 텍스쳐 UV 좌표 및 크기(xyzw), 텍스쳐 ID, 깊이, 색, 각도) | Make the batch square (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color, angle)
		void Draw(const glm::vec4& destRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const Vertex::ColorRGBA8& color, glm::vec2& direction); // 방향 정보가 추가된 배치를 입력받아 저장하는 함수 (사각형 위치 및 크기(xyzw), 텍스쳐 UV 좌표 및 크기(xyzw), 텍스쳐 ID, 깊이, 색, 방향) | Make the batch square (rectangle position with size (xyzw), UV position with size (xyzw), texture ID, depth, color, direction)

		void End(); // 스프라이트 입력을 종료시키는 함수 | End to make sprite

		void RenderBatch(); // 완성된 스프라이트를 화면에 출력하는 함수 | Draw the complete sprite on the screen

	private:
		void CreateRenderBatches(); // 출력할 배치를 만드는 함수 | Create the batch to render
		void CreateVertexArray(); // 버텍스 배열과 버퍼를 만드는 함수 | Create the VAO, VBO
		void SortGlyphs(); // 글리프를 정렬하는 함수 | Sort the Glyphs

		// 지정한 정렬방식에 따라 배치를 정렬하는 함수들 | Sorting the arrangement according to the specified alignment
		static bool CompareFrontToBack(Glyph* a, Glyph* b);
		static bool CompareBackToFront(Glyph* a, Glyph* b);
		static bool CompareTexture(Glyph* a, Glyph* b);

		GLuint _vbo; // VBO 번호 | VBO ID
		GLuint _vao; // VAO 번호 | VAO ID

		GlyphSortType _sortType; // 배치 정렬 방식 | Batch Sort Type

		std::vector<Glyph*> _glyphPointers; // 글리프 정렬용 포인터 | Glyph ID pointer for sorting glyphs
		std::vector<Glyph> _glyphs; // 글리프 정보 | Glyph ID
		std::vector<BatchRender> _renderBatches; // 배치 렌더 정보 | Batch render ID for render batches
	};
}
