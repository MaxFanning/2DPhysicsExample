#ifndef VERTEX_HEADER_H_
#define VERTEX_HEADER_H_

#include <XEngine.h>

class PhysicsBody;

class Vertex
{
public:
	Vertex(PhysicsBody* body, float posX, float posY, float mass);

	PhysicsBody* mParent;

	X::Math::Vector2 mPosition;
	X::Math::Vector2 mOldPosition;
	X::Math::Vector2 mAcceleration;

	float mMass;
	float mMu;

	X::Math::Vector2 mFriction;

private:

};

#endif
