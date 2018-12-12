#include "cFollowObjectCommand.h"

#include <iostream>
#include "Camera.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>





void cFollowObjectCommand::Initialize(std::vector<sNVPair> vecNVPairs)
{
	// Pull the data out of the vector
	this->theObj = vecNVPairs[0].pMeshObj;
	this->idealRelPosition = vecNVPairs[1].v3Value;	
	this->minDistance = vecNVPairs[2].fValue;	
	this->maxSpeedDistance = vecNVPairs[3].fValue;		
	this->maxSpeed = vecNVPairs[4].fValue;		
	this->targetObj = vecNVPairs[5].pMeshObj;
	this->time = vecNVPairs[6].fValue;
	this->b_Started = false;

	return;
}

void cFollowObjectCommand::Update(double deltaTime)
{
	if (!this->b_Started)
	{
		this->b_Started = true;
		this->initialTime = glfwGetTime();
		this->initPosition = this->theObj->position;
	}

	this->elapsedTime = glfwGetTime() - this->initialTime;

	this->finalPosition = this->targetObj->position;

	this->idealPosition = this->finalPosition + this->idealRelPosition;

	glm::vec3 direction = this->idealPosition - this->theObj->position;
	float distance = glm::length(direction);

	glm::vec3 directionNormal = glm::normalize(direction);





	float speedRatio = glm::smoothstep(this->minDistance,
		this->maxSpeedDistance,
		distance);

	float scaledMaxSpeed = speedRatio * this->maxSpeed;

	glm::vec3 vecMaxSpeed = glm::vec3(scaledMaxSpeed, scaledMaxSpeed, scaledMaxSpeed);

	glm::vec3 velocity = directionNormal * vecMaxSpeed;

	
	glm::vec3 deltaPosition = (float)deltaTime * velocity;

	this->theObj->position += deltaPosition;

	if (theObj->friendlyName == "cameraObj")
	{
		camera.Position = theObj->position;
		camera.SetViewMatrix(glm::lookAt(camera.Position, this->targetObj->position, glm::vec3(0.0f, 1.0f, 0.0f)));
		camera.b_controlledByScript = true;

	}



	if (this->elapsedTime > time && this->time != 0.0f) { this->m_bIsDone = true; };
	return;
}




bool cFollowObjectCommand::isFinished(void)
{
	if (this->m_bIsDone) { 

		 camera.b_controlledByScript = false; 
		 return true;
	}
	else { return false; }

}
