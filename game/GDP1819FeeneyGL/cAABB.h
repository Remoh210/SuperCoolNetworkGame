#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <map>
#include <vector>

// 10x10x10		 

// We picked this because it's the "least useful" one... 
class cAABB 
{			
public:
//	cAABB();
	void setSideLegth( float newLength )
	{
		this->sideLegth = newLength;
	}
	float getSideLength(void)
	{
		return this->sideLegth;
	}
	void setMinXYZ( glm::vec3 newMinXYZ )
	{
		this->minXYZ = newMinXYZ;
	}
	// Getters and setters
	glm::vec3 getCentre(void);
	glm::vec3 getMinXYZ(void)
	{
		return this->minXYZ;
	}

	unsigned long long getID(void);

	// This will return the "raw" ID value
	// (ignoring the side length)
	// NOTE: These are static so that they can be called by 
	//	anything, any time, without a specific AABB
	// (There are no "this" variables allowed in static)
	static unsigned long long generateID(glm::vec3 minXYZ);
	// This one will "clamp" the values to the length
	static unsigned long long generateID(glm::vec3 minXYZ, float AABBsideLength);

	// Store triangle "lookup" info
	struct sAABB_Triangle
	{
		// What information do you want to store here...? 
		glm::vec3 verts[3];
		// Why not other stuff?
		// Normal, etc.
	};
	std::vector< sAABB_Triangle > vecTriangles;		// CON: Triangles stored in multiple places, maybe?
//	std::vector< cTriangle* > vec_pTriangles;   // CON: Need central store of triangles
//	std::vector< cThingWeNeedToTestForCollision > 

private:
	float sideLegth;
	glm::vec3 minXYZ;
};

class cAABBHierarchy
{
//public:

public:		// Will be private
	// 
	std::map< unsigned long long,  /*ID of the AABB*/
		      cAABB* > m_mapAABBs;

	// If pointers to the "central store of triangles", 
	//  THIS would be the central store... 
	//	std::vector< cTriangle > vecTriangles;


};
