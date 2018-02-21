#ifndef PHYSICS_BODY_HEADER_H_
#define PHYSICS_BODY_HEADER_H_
#include <XEngine.h>
#include "Edge.h"

class Physics;
class Vertex;

class PhysicsBody
{
public:
	PhysicsBody();

	X::Math::Vector2 mCenter; //Center of mass
	int mMinX, mMinY, mMaxX, mMaxY; //Min/max coordinates of the bounding box
	bool mIsActive;
	bool mHasPassedHUD;
	Physics* mWorld = nullptr;

	std::vector<Vertex> mVertices;
	Vertex* mVertexCM;
	std::vector<Edge> mEdges;

	void AddEdge(const Edge& edge);
	void AddVertex(const Vertex& vert);
	void ProjectToAxis(X::Math::Vector2 &axis, float& min, float& max);
	void Render();
	void UpdateForces();
	void CalculateCenter(); //Calculates the center of mass
	void CheckIfPassedHUD();

private:

};

#endif
