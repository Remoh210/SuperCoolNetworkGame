#include "cMoveToEaseIO.h"
#include "Camera.h"
#include <iostream>
#include <GLFW/glfw3.h>



cMoveToEaseIO::cMoveToEaseIO()
{
	this->initPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	this->finalPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	this->direction = glm::vec3(0.0f, 0.0f, 0.0f);
	this->distanceToTarget = 0.0f;
	this->Speed = 0.0f;
	this->initialTime = 0;
	this->elapsedTime = 0;
	this->AccRate = 0.1f;
	this->DecRate = 0.9f;
	this->b_first = false;
	this->m_bIsDone = false;

	return;
}


void cMoveToEaseIO::Initialize(std::vector<sNVPair> vecNVPairs)
{
	this->objToMove = vecNVPairs[0].pMeshObj;
	this->finalPosition = vecNVPairs[1].v3Value;
	this->Time = vecNVPairs[2].fValue;

	if (vecNVPairs[3].fValue != 0) this->AccRate = vecNVPairs[3].fValue;
	if (vecNVPairs[4].fValue != 0) this->DecRate = vecNVPairs[4].fValue;
	if (vecNVPairs[5].pMeshObj != nullptr) { this->finalPosition = vecNVPairs[5].pMeshObj->position; }

	return;
}

void cMoveToEaseIO::Update(double deltaTime)
{

	if (this->b_first == false)
	{


		this->initialTime = glfwGetTime();
		this->b_first = true;
		this->initPosition = this->objToMove->position;
		this->direction = glm::normalize(finalPosition - initPosition);
		this->distanceToTarget = glm::distance(finalPosition, initPosition);
		this->Speed = (float)this->distanceToTarget / this->Time;


	}

	// Calculate remaining distance
	float remainingDistance = glm::distance(this->finalPosition, this->objToMove->position);

	this->elapsedTime = glfwGetTime() - this->initialTime;

	float range1 = this->AccRate * this->Time;
	float range2 = this->DecRate * this->Time;


	float SmoothRate = glm::smoothstep(-0.1f, range1, (float)this->elapsedTime) * (1 - glm::smoothstep(range2, this->Time, (float)this->elapsedTime));

	SmoothRate = SmoothRate * this->Speed;

	glm::vec3 velocity = this->direction * SmoothRate;

	glm::vec3 deltaPosition = (float)deltaTime * velocity;

	glm::vec3 nextPosition = this->objToMove->position += deltaPosition;
	float nextDistance = glm::distance(finalPosition, nextPosition);


	if (nextDistance > remainingDistance)
	{ 
		this->objToMove->position = this->finalPosition;
	}
	else
	{
		this->objToMove->position += deltaPosition;
		if (objToMove->friendlyName == "cameraObj") { camera.Position = objToMove->position; }
	}

	return;
}


bool cMoveToEaseIO::isFinished(void)
{
	if (this->m_bIsDone) return true;

	
	if (this->objToMove->position == this->finalPosition)
	{
		this->m_bIsDone = true;
		//this->objToMove->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		return true;
	}
	else
		return false;
}


