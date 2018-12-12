#include "cMoveToEase.h"
#include <iostream>


void cMoveToEase::Initialize(std::vector<sNVPair> vecNVPairs)
{
	this->objToMove = vecNVPairs[0].pMeshObj;
	this->destination = vecNVPairs[1].v3Value;
	this->time = vecNVPairs[2].fValue;
	this->slowDownPer = vecNVPairs[3].fValue;

	this->speed = glm::distance(this->destination, this->objToMove->position) / this->time;
	this->start = this->objToMove->position;

	this->currentLocation = objToMove->position;
	return;
}



void cMoveToEase::Update(double deltaTime)
{
	float currentDistance = glm::distance(this->destination, this->currentLocation);

	if (currentDistance <= this->minDistance)
	{
		this->m_bIsDone = true;
		return;
	}

	// Here, I am NOT where I'm supposed to be, so I need to move.
	// Calculate the direction vector to the target location
	glm::vec3 vDirection = this->destination - this->currentLocation;

	// To calc velocity, I will normalize
	glm::vec3 vVel = glm::normalize(vDirection);


	// Adjust velocity in the "slow down" zone
//	vVel = this->m_adjustVelocity( vVel );
	vVel = this->m_adjustVelocity(vVel);


	// How far to move in THIS time step?
	glm::vec3 deltaMove = vVel * (float)deltaTime;

	objToMove->position += deltaMove;

	currentLocation += deltaMove;
	this->objToMove->position += deltaMove;

	//this->objToMove->position += velocity * (float)deltaTime;
	std::cout << "Pos x: " << this->objToMove->position.x << "Pos y: " << this->objToMove->position.y << "Pos z: " << this->objToMove->position.z << std::endl;
	std::cout << "vel x: " << vVel.x << " Vel y : " << vVel.y << " Vel z : " << vVel.z << std::endl;

	return;
}



glm::vec3 cMoveToEase::m_adjustVelocity(glm::vec3 vVel)
{

	float wholeDis = glm::distance(this->destination, this->currentLocation);
	//float MaxSpeedDis = wholeDis - wholeDis / 100 * this->slowDownPer;
	float MaxSpeedDis = 100;

	float currentDistance = glm::distance(this->currentLocation, this->destination);

	float distance = clamp(this->minDistance, MaxSpeedDis, currentDistance);

	// Now distance will be MAX this->maxSpeedDistance...
	// Scale this from 0 to 1 (1 at max distance)
	float slowDownRatio = distance / (MaxSpeedDis - this->minDistance);

	// Smoothstep
	slowDownRatio = smoothstep(0.0f, 1.0f, slowDownRatio);

	//	std::cout << "slowDownRatio = " << slowDownRatio << std::endl;

	vVel *= (slowDownRatio * this->speed);

	return vVel;
}




bool cMoveToEase::isFinished(void)
{
	return this->m_bIsDone;
}
