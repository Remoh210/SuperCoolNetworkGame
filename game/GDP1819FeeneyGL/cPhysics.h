#ifndef _cPhysics_HG_
#define _cPhysics_HG_

// Handles all things related to Physics collision detection and response

#include "Physics_Types.h"
#include <vector>

typedef glm::vec3 Point;
typedef glm::vec3 Vector;


class cPhysics
{
public:
	cPhysics();
	~cPhysics();

	// Passes and returns the physical parameters
	void DoPhysicsUpdate( double fDeltaTime, std::vector< Physics::sPhysParams* > &vec_pPhysObjects );

	Point ClosestPtPointTriangle(Point p, Point a, Point b, Point c);

};

#endif
