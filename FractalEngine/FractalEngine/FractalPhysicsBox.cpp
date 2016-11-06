#include "FractalPhysicsBox.h"


/* - Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start - */
Fractal::PhysicsBox::PhysicsBox()
{

}


/* - Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end - */
Fractal::PhysicsBox::~PhysicsBox()
{

}


void Fractal::PhysicsBox::Init(b2World* newWorld, glm::vec2 newPosition, glm::vec2 newDimension, Fractal::Vertex::ColorRGBA8 newColor)
{
	// �ڽ� �Ӽ� ����
	dimension = newDimension;
	color = newColor;

	// �ڽ��� �� ��ü ����� | Make the body
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