#pragma once
#include <BOX2D\Box2D.h>
#include <GLM\glm.hpp>
#include "FractalVertex.h"


namespace Fractal
{
	class PhysicsBox
	{
	public:
		PhysicsBox(); // 클래스 시작시 호출되는 함수 | Function called when the class start
		~PhysicsBox(); // 클래스 종료시 호출되는 함수 | Function called when the class end

		void Init(b2World* newWorld, glm::vec2 newPosition, glm::vec2 newDimension, Fractal::Vertex::ColorRGBA8 newColor);

		b2Body* GetBody() const { return body; }
		b2Fixture* GetFixture() const { return fixture; }
		const glm::vec2& getDimensions() const { return dimension; }
		const Fractal::Vertex::ColorRGBA8& getColor() const { return color; }

	private:
		b2Body* body = nullptr;
		b2Fixture* fixture = nullptr;

		glm::vec2 dimension;
		Fractal::Vertex::ColorRGBA8 color;
	};
}