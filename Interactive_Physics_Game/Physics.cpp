#include "Physics.h"
#include "PhysicsBody.h"
#include "Vertex.h"
#include "Edge.h"
#include "WinMain.h"

void Physics::Update()
{
	UpdateForces();
	UpdateVerlet();
	IterateCollisions();
}

void Physics::Render()
{
	for (auto &body : mBodies)
	{
		body.Render();
	}
}

void Physics::AddBody(PhysicsBody body) //Adds new elements to the simulation
{
	mBodies.push_back(body);
}

Math::Vector2 Physics::GetGravity()
{
	return mGravity;
}

Vertex* Physics::FindVertex(int x, int y)
{
	Vertex *nearestVertex = nullptr;
	float minDistance = 1000.0f;
	int bodyCount = mBodies.size();

	Math::Vector2 coords((float)x, (float)y);

	for (auto &body : mBodies)
	{
		for (auto &vertex : body.mVertices)
		{
			float distance = (vertex.mPosition - coords).LengthSq();

			if (distance < minDistance)
			{
				nearestVertex = &vertex;
				minDistance = distance;
			}
		}

	}

	return nearestVertex;
}

void Physics::UpdateForces()
{
	for (auto &body : mBodies)
	{
		body.UpdateForces();
	}
}

void Physics::UpdateVerlet()
{
	for (auto &body : mBodies)
	{
		for (auto &vertex : body.mVertices)
		{
			Vertex& cmVertex = *body.mVertexCM;

			Math::Vector2 pos = vertex.mPosition;
			pos.x = MAX(MIN(pos.x, (float)GWidth), 0.0f);
			pos.y = MAX(MIN(pos.y, (float)GHeight), 0.0f);

			Math::Vector2 temp = vertex.mPosition;
			Math::Vector2 posDiff = vertex.mPosition - vertex.mOldPosition;

			vertex.mPosition += posDiff + vertex.mAcceleration * mTimestep * mTimestep;

			//--------------Applying Friction------------------

			if (pos.y == GHeight)
			{
				if (posDiff.x > 0.0f) //going right
				{
					if ((cmVertex.mPosition.x - cmVertex.mOldPosition.x) >= 0.5f)
					{
						cmVertex.mPosition -= (cmVertex.mFriction * mTimestep * mTimestep);
					}
					else
					{
						cmVertex.mPosition.x = cmVertex.mOldPosition.x;
						temp.x = vertex.mPosition.x;
					}
				}
				else if (posDiff.x < 0.0f) //going left
				{
					if ((cmVertex.mPosition.x - cmVertex.mOldPosition.x) <= -0.5f)
					{
						cmVertex.mPosition += (cmVertex.mFriction * mTimestep * mTimestep);
					}
					else
					{
						cmVertex.mPosition.x = cmVertex.mOldPosition.x;
						temp.x = vertex.mPosition.x;
					}
				}
			}

			vertex.mOldPosition = temp;
		}
	}
}

void Physics::UpdateEdges()
{
	for (auto &body : mBodies)
	{
		for (auto &edge : body.mEdges)
		{
			Math::Vector2 vec1Vec2 = edge.mVector2->mPosition - edge.mVector1->mPosition; //Calculate the vector between the two vertices

			float vec1Vec2Length = vec1Vec2.Length(); //Calculate the current distance
			float Diff = vec1Vec2Length - edge.mLength; //Calculate the difference from the original length
			vec1Vec2.Normalize();

			edge.mVector1->mPosition += vec1Vec2 * Diff * 0.5f; //Push both vertices apart by half of the difference respectively so the distance between them equals the original length
			edge.mVector2->mPosition -= vec1Vec2 * Diff * 0.5f;
		}
	}
}

void Physics::IterateCollisions()
{
	int bodyCount = mBodies.size();

	for (int i = 0; i < mIterations; i++)
	{ 
		for (auto &body : mBodies)
		{
			if (body.mHasPassedHUD == false)
			{
				body.CheckIfPassedHUD();
			}
			//if body hasnt passed through hud yet return
			if (body.mHasPassedHUD == true)
			{
				for (auto &vertex : body.mVertices)
				{
					Math::Vector2& pos = vertex.mPosition;

					pos.x = MAX(MIN(pos.x, (float)GWidth), 130.0f); //acount for hud
					pos.y = MAX(MIN(pos.y, (float)GHeight), 0.0f);
				}
			}
			else if (body.mHasPassedHUD == false)
			{
				for (auto &vertex : body.mVertices)
				{
					Math::Vector2& pos = vertex.mPosition;

					pos.x = MAX(MIN(pos.x, (float)GWidth), 0.0f); 
					pos.y = MAX(MIN(pos.y, (float)GHeight), 0.0f);
				}
			}
		}

		UpdateEdges(); //Edge correction step

		for (auto &body : mBodies)
		{
			body.CalculateCenter();
		}

		for (int i = 0; i < bodyCount; i++)
		{ //Iterate through all bodies
			for (int j = 0; j < bodyCount; j++)
			{
				if (i != j)
				{
					if (BodiesOverlap(&mBodies[i], &mBodies[j])) //Test the bounding boxes
					{
						if (DetectCollision(&mBodies[i], &mBodies[j])) //If there is a collision, respond to it
						{
							ProcessCollision();
						}
					}
				}
			}
		}
	}
}

bool Physics::DetectCollision(PhysicsBody* body1, PhysicsBody* body2)
{
	float minDistance = 10000.0f; //Initialize the length of the collision vector to a relatively large value
	for (int i = 0; i < body1->mEdges.size() + body2->mEdges.size(); i++)
	{ 
		Edge* edge;

		if (i < body1->mEdges.size())
		{
			edge = &body1->mEdges[i];
		}
		else
		{
			edge = &body2->mEdges[i - body1->mEdges.size()];
		}

		//This will skip edges that lie totally inside the bodies, as they don't matter.
		if (!edge->mBoundary)
		{
			continue;
		}

		Math::Vector2 axis(edge->mVector1->mPosition.y - edge->mVector2->mPosition.y, edge->mVector2->mPosition.x - edge->mVector1->mPosition.x); //Calculate the perpendicular to this edge and normalize it
		axis.Normalize();

		float minA, minB, maxA, maxB; //Project both bodies onto the perpendicular
		body1->ProjectToAxis(axis, minA, maxA);
		body2->ProjectToAxis(axis, minB, maxB);

		float distance = IntervalDistance(minA, maxA, minB, maxB); //Calculate the distance between the two intervals

		if (distance > 0.0f) //If the intervals don't overlap, return, since there is no collision
		{
			return false;
		}
		else if (abs(distance) < minDistance)
		{
			minDistance = abs(distance);

			CollisionInfo.mNormal = axis; //Save collision information for later
			CollisionInfo.mEdge = edge;    //Store the edge, as it is the collision edge
		}
	}

	CollisionInfo.mDepth = minDistance;

	if (CollisionInfo.mEdge->mParent != body2)
	{ //Ensure that the body containing the collision edge lies in B2 and the one conatining the collision vertex in B1
		PhysicsBody* temp = body2;
		body2 = body1;
		body1 = temp;
	}

	int sign = SGN(CollisionInfo.mNormal.Dot((body1->mCenter - body2->mCenter))); //*(body1->mCenter - body2->mCenter)); //This is needed to make sure that the collision normal is pointing at B1
																//Remember that the line equation is N*( R - R0 ). We choose B2->Center as R0; the normal N is given by the collision normal
	if (sign != 1)
	{
		CollisionInfo.mNormal = -CollisionInfo.mNormal; //Revert the collision normal if it points away from B1
	}

	Math::Vector2 collisionVector = CollisionInfo.mNormal * CollisionInfo.mDepth;

	float smallestD = 10000.0f; //Initialize the smallest distance to a large value
	for (auto &vertex: body1->mVertices)
	{
		float distance = CollisionInfo.mNormal.Dot((vertex.mPosition - body2->mCenter)); //Measure the distance of the vertex from the line using the line equation

		if (distance < smallestD)
		{ //If the measured distance is smaller than the smallest distance reported so far, set the smallest distance and the collision vertex
			smallestD = distance;
			CollisionInfo.mVertex = &vertex;
		}
	}

	return true; //There is no separating axis. Report a collision!
}

void Physics::ProcessCollision()
{
	Vertex* edge1 = CollisionInfo.mEdge->mVector1;
	Vertex* edge2 = CollisionInfo.mEdge->mVector2;

	Math::Vector2 collisionVector = CollisionInfo.mNormal * CollisionInfo.mDepth;

	float t;
	if (abs(edge1->mPosition.x - edge2->mPosition.x) > abs(edge1->mPosition.y - edge2->mPosition.y))
	{
		t = (CollisionInfo.mVertex->mPosition.x - collisionVector.x - edge1->mPosition.x) / (edge2->mPosition.x - edge1->mPosition.x);
	}
	else
	{
		t = (CollisionInfo.mVertex->mPosition.y - collisionVector.y - edge1->mPosition.y) / (edge2->mPosition.y - edge1->mPosition.y);
	}

	float lambda = 1.0f / (t * t + (1 - t) * (1 - t));

	edge1->mPosition -= (collisionVector * (1 - t) * 0.5f * lambda);
	edge2->mPosition -= (collisionVector * t * 0.5f * lambda);

	CollisionInfo.mVertex->mPosition += collisionVector * 0.5f;
}

float Physics::IntervalDistance(float minA, float maxA, float minB, float maxB)
{
	if (minA < minB)
	{
		return minB - maxA;
	}
	else
	{
		return minA - maxB;
	}
}

bool Physics::BodiesOverlap(PhysicsBody* body1, PhysicsBody* body2) //Used for optimization to test if the bounding boxes of two bodies overlap
{
	return (body1->mMinX <= body2->mMaxX) && (body1->mMinY <= body2->mMaxY) && (body1->mMaxX >= body2->mMinX) && (body2->mMaxY >= body1->mMinY);
}

void Physics::CreateBox(bool active, int x, int y, int width, int height, float mass)
{
	PhysicsBody tempBody;
	mBodies.push_back(tempBody);

	PhysicsBody& body = mBodies.back();

	body.mIsActive = active;

	Vertex vector1 = (Vertex(&body, x, y, 1.0f));
	Vertex vector2 = (Vertex(&body, x + width, y, 1.0f));
	Vertex vector3 = (Vertex(&body, x + width, y + height, 1.0f));
	Vertex vector4 = (Vertex(&body, x, y + height, 1.0f));

	body.AddVertex(vector1);
	body.AddVertex(vector2);
	body.AddVertex(vector3);
	body.AddVertex(vector4);

	body.CalculateCenter();
	Vertex vectorCM = (Vertex(&body, body.mCenter.x, body.mCenter.y, mass));
	body.AddVertex(vectorCM);
	body.mVertexCM = &body.mVertices[4];

	//outside edges
	body.AddEdge(Edge(&body, &body.mVertices[0], &body.mVertices[1], true));
	body.AddEdge(Edge(&body, &body.mVertices[1], &body.mVertices[2], true));
	body.AddEdge(Edge(&body, &body.mVertices[2], &body.mVertices[3], true));
	body.AddEdge(Edge(&body, &body.mVertices[3], &body.mVertices[0], true));

	//cm edges
	body.AddEdge(Edge(&body, body.mVertexCM, &body.mVertices[0], false));
	body.AddEdge(Edge(&body, body.mVertexCM, &body.mVertices[1], false));
	body.AddEdge(Edge(&body, body.mVertexCM, &body.mVertices[2], false));
	body.AddEdge(Edge(&body, body.mVertexCM, &body.mVertices[3], false));

	//inside edges
	body.AddEdge(Edge(&body, &body.mVertices[0], &body.mVertices[2], false));
	body.AddEdge(Edge(&body, &body.mVertices[1], &body.mVertices[3], false));

	body.mWorld = this;
}

void Physics::CreateTriangle(bool active, int x, int y, int width, int height, float mass)
{
	PhysicsBody tempBody;
	mBodies.push_back(tempBody);

	PhysicsBody& body = mBodies.back();

	body.mIsActive = active;

	Vertex vector1 = (Vertex(&body, x, y, 1.0f));
	Vertex vector2 = (Vertex(&body, x - (width / 2), y + height, 1.0f));
	Vertex vector3 = (Vertex(&body, x + (width / 2), y + height, 1.0f));

	body.AddVertex(vector1);
	body.AddVertex(vector2);
	body.AddVertex(vector3);

	body.CalculateCenter();
	Vertex vectorCM = (Vertex(&body, body.mCenter.x, body.mCenter.y, mass));
	body.AddVertex(vectorCM);
	body.mVertexCM = &body.mVertices[3];

	//outside edges
	body.AddEdge(Edge(&body, &body.mVertices[0], &body.mVertices[1], true));
	body.AddEdge(Edge(&body, &body.mVertices[1], &body.mVertices[2], true));
	body.AddEdge(Edge(&body, &body.mVertices[2], &body.mVertices[0], true));

	//cm edges
	body.AddEdge(Edge(&body, body.mVertexCM, &body.mVertices[0], false));
	body.AddEdge(Edge(&body, body.mVertexCM, &body.mVertices[1], false));
	body.AddEdge(Edge(&body, body.mVertexCM, &body.mVertices[2], false));

	body.mWorld = this;
}
