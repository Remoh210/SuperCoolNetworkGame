#include "cPhysics.h"

// 
#include <thread>

cPhysics::cPhysics()
{

	return;
}

cPhysics::~cPhysics()
{
	return;
}






// Shapes
Physics::sSphere::sSphere()
{
	this->radius = 0.0f;
	return;
}

Physics::sSphere::sSphere(float initRadius)
{ 
	this->radius = initRadius; 
	return;
}

Physics::sTriangle::sTriangle()
{
	this->v[0] = glm::vec3(0.0f);
	this->v[1] = glm::vec3(0.0f);
	this->v[2] = glm::vec3(0.0f);
	return;
}

Physics::sTriangle::sTriangle( glm::vec3 v0, glm::vec3 v1, glm::vec3 v2 )
{
	this->v[0] = v0;
	this->v[1] = v1;
	this->v[2] = v2;
	return;
}

Physics::sTriangle::sTriangle( glm::vec3* pV )
{
	this->v[0] = pV[0];
	this->v[1] = pV[1];
	this->v[2] = pV[2];
	return;
}

Physics::sAABB::sAABB()
{
	this->maxXYZ = glm::vec3(0.0f);
	this->minXYZ = glm::vec3(0.0f);
	return;
}

Physics::sAABB::sAABB( glm::vec3 minXYZ, glm::vec3 maxXYZ )
{
	this->maxXYZ = maxXYZ;
	this->minXYZ = minXYZ;
	return;
}

Physics::sAOB::sAOB()
{
	this->centreXYZ = glm::vec3(0.0f);
	this->halfExtentsXYZ = glm::vec3(0.0f);
	this->matOrientation = glm::mat3(1.0f);
	return;
}

//cPhysics::sAOB::sAOB( glm::vec3 centreXYZ, glm::vec3 halfExtentsXYZ, glm::quat qOrientation );

Physics::sAOB::sAOB( glm::vec3 centreXYZ, glm::vec3 halfExtentsXYZ, glm::mat3 matOrientation )
{
	this->centreXYZ = centreXYZ;
	this->halfExtentsXYZ = halfExtentsXYZ;
	this->matOrientation = matOrientation;
	return;
}

Physics::sPlane::sPlane()
{
	this->positionXYZ = glm::vec3(0.0f);
	this->normalXYZ = glm::vec3(0.0f);
	return;
}

Physics::sPlane::sPlane( glm::vec3 positionXYZ, glm::vec3 normalXYZ )
{
	this->positionXYZ = positionXYZ;
	this->normalXYZ = normalXYZ;
	return;
}
