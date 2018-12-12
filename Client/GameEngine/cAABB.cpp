#include "cAABB.h"



unsigned long long cAABB::generateID(glm::vec3 minXYZ, float AABBsideLength)
{

	minXYZ.x = (floor(minXYZ.x / (float)AABBsideLength)) * (float)AABBsideLength;
	minXYZ.y = (floor(minXYZ.y / (float)AABBsideLength)) * (float)AABBsideLength;
	minXYZ.z = (floor(minXYZ.z / (float)AABBsideLength)) * (float)AABBsideLength;

	return cAABB::generateID(minXYZ);
}


unsigned long long cAABB::generateID(glm::vec3 minXYZ)
{
	// 16,000,000,100,000,100,000 
	//    +xx,xxx +yy,yyy +zz,zzz  
	// If the value is NEGATIVE, then we will set the 6th 
	// digit to 1. If +ve then, it's 0

	// for example, xyz of 20, 460, 1280 would give:
	// 	// 100020 000460 101280	

	unsigned long long theABS_X = (unsigned long long(fabs(minXYZ.x)));
	unsigned long long theABS_Y = (unsigned long long(fabs(minXYZ.y)));
	unsigned long long theABS_Z = (unsigned long long(fabs(minXYZ.z)));

	// Add the "sign" digit:
	// If +ve, then the sign is 0, eg: 193 would be:  000193   (000 193)
	// If -ve, then the sign is 1, eg: -193 would be: 100193   (100 193)
	if (minXYZ.x < 0.0f) { theABS_X += 100000; }	// Sets 6th digit to 1
	if (minXYZ.y < 0.0f) { theABS_Y += 100000; }	// Sets 6th digit to 1
	if (minXYZ.z < 0.0f) { theABS_Z += 100000; }	// Sets 6th digit to 1

	unsigned long long theID =
		theABS_Z
		+ (theABS_Y * 1000000)				// Shift the y to the 7th digit
		+ (theABS_X * 1000000 * 1000000);	// Shift the x to the 13th

	return theID;
}

unsigned long long cAABB::getID(void)
{
	return cAABB::generateID(this->getMinXYZ(), this->getSideLength());
}
