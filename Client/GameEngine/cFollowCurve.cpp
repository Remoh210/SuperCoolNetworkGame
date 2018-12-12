#include "cFollowCurve.h"

#include <iostream>
#include "Camera.h"
#include <GLFW/glfw3.h>


cFollowCurve::cFollowCurve()
{
	this->b_Started = false;
	this->m_bIsDone = false;
}


float getPoint(float n1, float n2, float perc)
{
	float diff = n2 - n1;

	return n1 + (diff * perc);
}

float curveLength(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
{


	std::vector< glm::vec3 > initialPoints;
	initialPoints.push_back(p0);
	initialPoints.push_back(p1);
	initialPoints.push_back(p2);

	std::vector< glm::vec3 > segments;

	glm::vec3 segment1 = initialPoints[1] - initialPoints[0];
	segments.push_back(segment1);

	glm::vec3 segment2 = initialPoints[0] - 2.0f * initialPoints[1] + initialPoints[2];
	segments.push_back(segment2);

	float length;

	if (segments[1] != glm::vec3(0.0f))
	{
		

		float c = 4.0f * glm::dot(segments[1], segments[1]); 
		float b = 8.0f * glm::dot(segments[0], segments[1]);
		float a = 4.0f * glm::dot(segments[0], segments[0]);
		float q = 4.0f * a * c - b * b; 

		float twoCpB = 2.0f * c + b;
		float sumCBA = c + b + a;
		float mult0 = 0.25f / c;
		float mult1 = q / (8.0f * powf(c, 1.5f));
		length =
			mult0 * (twoCpB * sqrtf(sumCBA) - b * sqrtf(a)) +
			mult1 * (logf(2.0f * sqrtf(c * sumCBA) + twoCpB) - logf(2.0f * sqrtf(c * a) + b));
	}
	else
	{
		length = 2.0f * glm::length(initialPoints[1] - initialPoints[0]);
	}

	return length;
}

void cFollowCurve::Initialize(std::vector<sNVPair> vecNVPairs)
{

	// Pull the data out of the vector
	this->theObj = vecNVPairs[0].pMeshObj;
	this->controlPoint = vecNVPairs[1].v3Value;
	this->finalPosition = vecNVPairs[2].v3Value;
	this->time = vecNVPairs[3].fValue;
	this->targetObj = vecNVPairs[4].pMeshObj;

	if (this->targetObj != nullptr) { this->finalPosition = this->targetObj->position; }

	this->direction = glm::normalize(finalPosition - initPosition);
	this->distanceToTarget = glm::distance(finalPosition, initPosition);
	

	this->distanceCurveLenght = curveLength(this->initPosition, this->controlPoint, this->finalPosition);

	this->speed = distanceCurveLenght / time;

}

void cFollowCurve::Update(double deltaTime)
{


	if (this->b_Started == false)
	{
		this->initialTime = glfwGetTime();
		this->b_Started = true;
		this->initPosition = this->theObj->position;
		this->prevPosition = this->initPosition;
	}



	this->elapsedTime = glfwGetTime() - this->initialTime;


	if (this->theObj->position != this->prevPosition)
	{ 
		this->traveledDistance += glm::distance(this->theObj->position, this->prevPosition);
	}


	this->prevPosition = this->theObj->position;


	glm::vec3 p1 = this->initPosition;
	glm::vec3 p2 = this->controlPoint;
	glm::vec3 p3 = this->finalPosition;


	float ratio = this->elapsedTime / this->time;

	float i = 0;

	if (ratio > 1.0f)
		i = 1.0f;
	else
		i = ratio;

	float xa = getPoint(p1.x, p2.x, i);
	float ya = getPoint(p1.y, p2.y, i);
	float za = getPoint(p1.z, p2.z, i);

	float xb = getPoint(p2.x, p3.x, i);
	float yb = getPoint(p2.y, p3.y, i);
	float zb = getPoint(p2.z, p3.z, i);

	float x = getPoint(xa, xb, i);
	float y = getPoint(ya, yb, i);
	float z = getPoint(za, zb, i);

	this->theObj->position = glm::vec3(x, y, z);
	if (theObj->friendlyName == "cameraObj") 
	{
		camera.Position = theObj->position; 
	}
	



	return;
}

bool cFollowCurve::isFinished()
{
	if (this->m_bIsDone) return true;

	if (glm::distance (this->theObj->position, this->finalPosition) < 1.0f)
	{
		this->m_bIsDone = true;
		return true;
	}
	else
	{
		return false;
	}
}


