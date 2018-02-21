#include "Edge.h"
#include "PhysicsBody.h"
#include "Vertex.h"

Edge::Edge(PhysicsBody* body, Vertex* v1, Vertex* v2, int boundary)
{
	mVector1 = v1; //Set boundary vertices
	mVector2 = v2;

	mLength = (v2->mPosition - v1->mPosition).Length(); //Calculate the original length
	mBoundary = boundary;

	mParent = body;
}