#include "Vertex.h"
#include "PhysicsBody.h"

Vertex::Vertex(PhysicsBody* body, float posX, float posY, float mass)
{
	mPosition = X::Math::Vector2(posX, posY);
	mOldPosition = X::Math::Vector2(posX, posY);

	mMass = mass;
	mParent = body;
	mMu = 0.05f;

	if (mMass != 1)
	{
		mFriction.x = mMu * (mMass * 10.0f);
	}
	else
	{
		mFriction.x = 0.0f;
	}
	mFriction.y = 0.0f;

	mMass = 1;
	mParent = body;
}