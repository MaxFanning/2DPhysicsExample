#include "PhysicsBody.h"
#include "Vertex.h"
#include "WinMain.h"
#include "Physics.h"

PhysicsBody::PhysicsBody()
{
	mIsActive = false;
	mHasPassedHUD = false;
}

void PhysicsBody::AddEdge(const Edge& edge)
{
	mEdges.push_back(edge);
}

void PhysicsBody::AddVertex(const Vertex& vert)
{
	mVertices.push_back(vert);
}

void PhysicsBody::ProjectToAxis(X::Math::Vector2 &axis, float& min, float& max)
{
	float dotP = axis.Dot(mVertices[0].mPosition);

	min = max = dotP; //Set the minimum and maximum values to the projection of the first vertex

	for (auto &vertex : mVertices)
	{
		dotP = axis.Dot(vertex.mPosition); //*mVertices[i]->mPosition; //Project the rest of the vertices onto the axis and extend the interval to the left/right if necessary

		min = MIN(dotP, min);
		max = MAX(dotP, max);
	}
}

void PhysicsBody::CheckIfPassedHUD()
{
	bool hasPassed = true;

	for (auto& vertex : mVertices)
	{
		if (vertex.mPosition.x <= 130)
		{
			hasPassed = false;
		}
	}

	mHasPassedHUD = hasPassed;
}

void PhysicsBody::CalculateCenter() //Calculates the center of mass
{
	mCenter = X::Math::Vector2(0.0f, 0.0f);

	mMinX = mMinY = 10000.0f;
	mMaxX = mMaxY = -10000.0f;

	for (auto &vertex : mVertices)
	{
		mCenter += vertex.mPosition;

		mMinX = MIN(mMinX, vertex.mPosition.x);
		mMinY = MIN(mMinY, vertex.mPosition.y);
		mMaxX = MAX(mMaxX, vertex.mPosition.x);
		mMaxY = MAX(mMaxX, vertex.mPosition.y);
	}

	mCenter /= mVertices.size();
}

void PhysicsBody::UpdateForces()
{
	for (auto &vertex : mVertices)
	{
		X::Math::Vector2 gravity = mWorld->GetGravity();
		vertex.mAcceleration.x = gravity.x;
		vertex.mAcceleration.y = gravity.y;
	}
}

void PhysicsBody::Render()
{
	for (auto &edge : mEdges)
	{
		X::DrawScreenLine(edge.mVector1->mPosition, edge.mVector2->mPosition, X::Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	}

	for (auto &vertex : mVertices)
	{
		X::DrawScreenCircle(vertex.mPosition, 1.5f, X::Math::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	}
}