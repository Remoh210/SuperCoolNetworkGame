#ifndef _cMeshObject_HG_
#define _cMeshObject_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>	// Note strange folder
#include <string>
#include <vector>

// Add the ability to add debug shapes to the debug renderer
// **NOTE** it's the INTERFACE that I have, NOT the class
#include "DebugRenderer/iDebugRenderer.h"

struct sSphere
{
	sSphere( float theRadius )
	{
		this->radius = theRadius;
	}
	float radius;
};
struct sTriangle
{
	glm::vec3 v[3];
};
struct sAABB
{
	glm::vec3 minXYZ;
	glm::vec3 maxXYZ;
};

struct sAOB  
{
	glm::vec3 minXYZ;
	glm::vec3 maxXYZ;
	//glm::vec3 orientationEuler;
	glm::mat3 matOrientation;
};

struct sTextureInfo 
{ 
      std::string name; 
      int cachedTextureID; 	// Set to -1 by default
      float strength;		// Set to 0.0f by default
};


class cMeshObject
{
public:
	cMeshObject();


//	glm::vec3 preRotation;		// Happens BEFORE translation (move)
////	glm::vec3 position;
//	glm::vec3 postRotation;		// Happens AFTER translation (move)
private:
	glm::quat m_meshQOrientation;		// Like a mat3x3 rotation matrix
public:
	glm::quat getQOrientation(void)	{	return this->m_meshQOrientation;	};
	void setQOrientation( glm::quat newOrientation )
	{
		this->m_meshQOrientation = newOrientation;
	}
	void setMeshOrientationEulerAngles( glm::vec3 newAnglesEuler, bool bIsDegrees = false );
	void setMeshOrientationEulerAngles( float x, float y, float z, bool bIsDegrees = false );
	void adjMeshOrientationEulerAngles( glm::vec3 adjAngleEuler, bool bIsDegrees = false );
	void adjMeshOrientationEulerAngles( float x, float y, float z, bool bIsDegrees = false );
	void adjMeshOrientationQ( glm::quat adjOrientQ );


	void setUniformScale(float scale);
	glm::vec3 nonUniformScale;

	std::string meshName;	// Model to draw


	bool bIsWireFrame;	//  = true;  C++
	bool bIsVisible;

	// ignore this for now...
	// Around its own axis (NOT the origin)
	//glm::vec3 orientation;

//	glm::vec3 objColour;
	glm::vec4 materialDiffuse;		// RGB+Alpha
	void setDiffuseColour(glm::vec3 newDiffuse);
	void setAlphaTransparency(float newAlpha);
	glm::vec4 materialSpecular;		// RGB+specular power
	void setSpecularColour(glm::vec3 colourRGB);
	void setSpecularPower(float specPower);

	bool bUseVertexColour;		// = false (default)

	bool bDontLight;		// If true, just object colour is used

	// Child objects inside this one
	std::vector< cMeshObject* > vec_pChildObjectsToDraw;

	// Things we can use to find this object 
	std::string friendlyName;		// Human "Bob" "Ali" "Player"

	// Things that are updated by physics
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 accel;		// acceleration
	
	bool bIsUpdatedByPhysics;	// Phsyics "owns" the position	

	float mass;
	float inverseMass;

	// Assume that everything is a sphere...
	enum eShape
	{
		UNKOWN_SHAPE,
		SPHERE,
		TRIANGLE,
		AABB, AOB	// DON'T use these	
		// PLANE, AABB, etc., etc.
	};
	// Allows me to point to anything (any type)
	void* pTheShape;		// Details
	eShape shapeType;		// What shape it is

	//float radius;

	// 
	iDebugRenderer* pDebugRenderer;
	void Update(double deltaTime);

	// In theory, it's faster. 
	// Not likely. Suggestion, not a command
	inline unsigned int getUniqueID(void)
	{
		return this->m_uniqueID;
	}


	// Textures...
	std::vector<sTextureInfo> vecTextures;

private:
	unsigned int m_uniqueID;			// Number that's unique to this instance
	// static:
	// - There's only 1.
	// - All classes "see" it
	static unsigned int m_NextID; //= 0;	Can't assign as is static

	static const unsigned int STARTING_ID_VALUE = 1000;



};

#endif 
