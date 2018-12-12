#ifndef _Physics_Types_HG_
#define _Physics_Types_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/quaternion.hpp>


namespace Physics
{
	// Assume that everything is a sphere...
	enum eShape
	{
		UNKOWN_SHAPE,
		SPHERE,
		TRIANGLE,
		AABB, AOB	// DON'T use these	
		// PLANE, AABB, etc., etc.
	};

	// This would be placed in any object that is to be physically simulated
	struct sPhysParams
	{
		glm::vec3 position;
		//glm::vec3 orientationEuler;
		glm::quat qOrientation;
		glm::vec3 velocity;
		glm::vec3 accel;		// acceleration
		bool bIsUpdatedByPhysics;	// Phsyics "owns" the position	
		float mass;
		float inverseMass;
		// Allows me to point to anything (any type)
		void* pTheShape;		// Details
		eShape shapeType;		// What shape it is
	};

	// Shape types
	struct sSphere
	{
		sSphere();
		sSphere(float initRadius);
		float radius;
	};
	struct sTriangle
	{
		sTriangle();
		sTriangle( glm::vec3 v0, glm::vec3 v1, glm::vec3 v2 );
		sTriangle( glm::vec3* pV );
		glm::vec3 v[3];
	};
	struct sAABB
	{
		sAABB();
		sAABB( glm::vec3 minXYZ, glm::vec3 maxXYZ );
		glm::vec3 minXYZ;
		glm::vec3 maxXYZ;
	};

	struct sAOB  
	{
		sAOB();
	//		sAOB( glm::vec3 centreXYZ, glm::vec3 halfExtentsXYZ, glm::quat qOrientation );
		sAOB( glm::vec3 centreXYZ, glm::vec3 halfExtentsXYZ, glm::mat3 matOrientation );
		glm::vec3 centreXYZ;
		glm::vec3 halfExtentsXYZ;
		//glm::vec3 orientationEuler;
		glm::mat3 matOrientation;
	};
	
	struct sPlane
	{
		sPlane();
		sPlane( glm::vec3 positionXYZ, glm::vec3 normalXYZ );
		glm::vec3 positionXYZ;
		glm::vec3 normalXYZ;
	};
};

#endif


