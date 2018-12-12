#pragma once
#include "iCommand.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

class cFollowCurve : public iCommand
{
public:
	cFollowCurve();

	virtual void Initialize(std::vector<sNVPair> vecNVPairs);
	virtual void Update(double deltaTime);
	virtual bool isFinished(void);

private:
	cMeshObject* theObj;
	cMeshObject* targetObj;

	glm::vec3 initPosition;
	glm::vec3 prevPosition;
	glm::vec3 finalPosition;
	glm::vec3 controlPoint;
	glm::vec3 direction;

	float traveledDistance;
	float distanceToTarget;
	float speed;
	
	float time;
	double initialTime;
	float elapsedTime;

	float distanceCurveLenght;

	bool b_Started;
	bool m_bIsDone;

};
