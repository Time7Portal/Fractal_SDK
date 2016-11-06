#include "FractalPhysicsBox.h"


/* - 클래스 시작시 호출되는 함수 | Function called when the class start - */
Fractal::PhysicsBox::PhysicsBox()
{

}


/* - 클래스 종료시 호출되는 함수 | Function called when the class end - */
Fractal::PhysicsBox::~PhysicsBox()
{

}


void Fractal::PhysicsBox::Init(b2World* newWorld, glm::vec2 newPosition, glm::vec2 newDimension, Fractal::Vertex::ColorRGBA8 newColor)
{
	// 박스 속성 저장
	dimension = newDimension;
	color = newColor;

	// 박스가 될 몸체 만들기 | Make the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(newPosition.x, newPosition.y);
	body = newWorld->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(newDimension.x / 2.0f, newDimension.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixture = body->CreateFixture(&fixtureDef);
}