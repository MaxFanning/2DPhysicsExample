#ifndef EDGE_HEADER_H_
#define EDGE_HEADER_H_

class PhysicsBody;
class Vertex;

class Edge
{
public:
	PhysicsBody* mParent = nullptr;

	Vertex* mVector1 = nullptr;
	Vertex* mVector2 = nullptr;

	float mLength;
	int mBoundary;

	Edge(PhysicsBody* body, Vertex* v1, Vertex* v2, int boundary = true);

private:

};

#endif
