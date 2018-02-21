#ifndef PHYSICS_HEADER_H_
#define PHYSICS_HEADER_H_
#include <XEngine.h>
#include <stdbool.h>
#include <vector>
using namespace X;

#define MAX_BODIES 512  //Maximum body/vertex/edgecount the physics simulation can handle
#define MAX_VERTICES 1024
#define MAX_EDGES 1024
#define MAX_BODY_VERTICES 64 //Maximum body/edge count a body can contain
#define MAX_BODY_EDGES 64

class PhysicsBody;
class Vertex;
class Edge;
class Physics
{
public:
	Physics(float gravitationX = 0.0f, float gravitationY = 0.0f, int iterations = 1) :
		mGravity(Math::Vector2(gravitationX, gravitationY)),
		mIterations(iterations), mTimestep(1.0f) {}

	Vertex* FindVertex(int x, int y);
	Math::Vector2 GetGravity();

	void Update();
	void Render();

	void AddBody(PhysicsBody body); //Adds new elements to the simulation
	void CreateBox(bool active, int x, int y, int width, int height, float mass); //Helper function to create a box primitive
	void CreateTriangle(bool active, int x, int y, int width, int height, float mass);

private:
	Math::Vector2 mGravity;
	std::vector<PhysicsBody> mBodies;
	float mTimestep;
	int mIterations;

	void  UpdateForces();
	void  UpdateVerlet();
	void  UpdateEdges();
	void  IterateCollisions();
	bool  DetectCollision(PhysicsBody* body1, PhysicsBody* body2);
	void  ProcessCollision();
	float IntervalDistance(float minA, float maxA, float minB, float maxB);
	bool  BodiesOverlap(PhysicsBody* body1, PhysicsBody* body2); //Used for optimization to test if the bounding boxes of two bodies overlap
	struct
	{
		float mDepth;
		Math::Vector2 mNormal;

		Edge* mEdge;
		Vertex* mVertex;
	} CollisionInfo;
};

#endif
