#include "cOrientTo.h"
#include <iostream>
#include "Camera.h"
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>



cOrientTo::cOrientTo()
{
	this->initialTime = 0;
	this->time;
	this->Up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->b_Started = false;
	this->b_IsDone = false;
}

cOrientTo::~cOrientTo()
{

}

void cOrientTo::Initialize(std::vector<sNVPair> vecNVPairs)
{
	this->theObj = vecNVPairs[0].pMeshObj;
	this->time = vecNVPairs[1].fValue;
	this->targetObj = vecNVPairs[2].pMeshObj;
}

void cOrientTo::Update(double deltaTime)
{

	if (this->b_Started == false)
	{
		this->b_Started = true;

		this->initPosition = this->theObj->position;

		this->lookDirection = this->targetObj->position - this->theObj->position;

		this->lookDirection = glm::normalize(this->lookDirection);

		this->finalOrientation = glm::inverse(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), -this->lookDirection, this->Up));

		this->initOrientation = this->theObj->getQOrientation();

		this->initialTime = glfwGetTime();
		//if (theObj->friendlyName == "cameraObj") { this->initOrientation = glm::quat_cast(camera.GetViewMatrix()); }
	}


	if (this->initPosition != this->theObj->position)
	{
		this->lookDirection = this->targetObj->position - this->theObj->position;

		this->lookDirection = glm::normalize(this->lookDirection);

		this->finalOrientation = glm::inverse(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), -this->lookDirection, this->Up));
	}



	this->elapsedTime = glfwGetTime() - this->initialTime;
	float factor = this->elapsedTime / this->time;


	glm::quat quatInterp = glm::mix(this->initOrientation, this->finalOrientation, factor);
	this->theObj->setQOrientation(quatInterp);

	if (theObj->friendlyName == "cameraObj")
	{
		this->finalOrientation = glm::lookAt(camera.Position, this->targetObj->position, this->Up);
		this->elapsedTime = glfwGetTime() - this->initialTime;
		float factor = this->elapsedTime / this->time;
		glm::quat quatInterp = glm::mix(this->initOrientation, this->finalOrientation, factor);
		//camera.Position = theObj->position;
		camera.SetViewMatrix(glm::mat4_cast(quatInterp));
		camera.b_controlledByScript = true;

	}

	

}

bool cOrientTo::isFinished()
{
	if (this->b_IsDone) { return true; }

	else
	{
		float matching = glm::dot(this->theObj->getQOrientation(), this->finalOrientation);
		if (abs(matching - 1.0) < 0.001)
		{
			this->b_IsDone = true;
			//camera.b_controlledByScript = false;
			return true;
		}
		else{ return false; }	
	}


}







