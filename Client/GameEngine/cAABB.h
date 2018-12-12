#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <map>
#include <vector>

class cAABB
{
public:


	//Setters
	void setCenter(glm::vec3 newCenter) { this->center = newCenter; }
	void setHalfLegth(float newHalfLength) { this->halfLegth = newHalfLength; }

	//Getters
	glm::vec3 getCentre(void) { return this->center; };
	glm::vec3 getMinXYZ(void) { return this->center - this->halfLegth; }
	float getSideLength(void) { return this->halfLegth * 2; }
	float getHalfLength(void) { return this->halfLegth; }


	//ID
	unsigned long long getID(void);
	static unsigned long long generateID(glm::vec3 minXYZ);
	static unsigned long long generateID(glm::vec3 minXYZ, float AABBsideLength);

	

	struct sAABB_Triangle
	{
		glm::vec3 verts[3];
	};
	std::vector< sAABB_Triangle > vecTriangles;

private:
	float halfLegth;
	glm::vec3 center;
};

class cAABBHierarchy
{
	//public:

public:		

	/*ID of the AABB*/
	std::map< unsigned long long,  
		cAABB* > m_mapAABBs;

};