#include "cMoveToCommand.h"
#include <iostream>


void cMoveToCommand::Initialize(std::vector<sNVPair> vecNVPairs)
{
	this->objToMove = vecNVPairs[0].pMeshObj;
	this->destination = vecNVPairs[1].v3Value;	
	this->time = vecNVPairs[2].fValue;	
	this->speed = glm::distance(this->destination, this->objToMove->position) / this->time;
	this->start = this->objToMove->position;
	return;
}



void cMoveToCommand::Update(double deltaTime)
{
	float currentDistance = glm::distance(this->objToMove->position, this->destination);

	if (currentDistance <= this->minDistance)
	{
		this->m_bIsDone = true;
		return;
	}

	glm::vec3 vDirection = this->destination - this->start;
	float distance = glm::length(vDirection);
	glm::vec3 directionNormalized = vDirection / distance;
	glm::vec3 velocity = directionNormalized * speed;

	this->objToMove->position += velocity * (float)deltaTime;
	std::cout << "Pos x: " << this->objToMove->position.x << "Pos y: " << this->objToMove->position.y << "Pos z: " << this->objToMove->position.z << std::endl;
	std::cout <<"vel x: " << velocity.x << " Vel y : " << velocity.y << " Vel z : " << velocity.z << std::endl;

	return;
}




bool cMoveToCommand::isFinished(void)
{
	return this->m_bIsDone;
}