#ifndef WIN_MAIN_HEADER_H
#define WIN_MAIN_HEADER_H

#include "Physics.h"
#include "PhysicsBody.h"
#include "Vertex.h"

#define MIN( a, b ) ( (a) < (b) ? (a) : (b) ) //Just two macros to determine the minimum/maximum of two values
#define MAX( a, b ) ( (a) > (b) ? (a) : (b) )
#define SGN( a ) ( (a) < 0 ? (-1) : (1) ) //Sign of A

extern int GWidth; //for Physics.cpp
extern int GHeight;

extern Physics World;

#endif
