#pragma once
#include "iCommand.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

class cMoveToEaseIO : public iCommand
{
public:
	cMoveToEaseIO();
	//~cMoveToEaseIO();

	virtual void Initialize(std::vector<sNVPair> vecNVPairs);
	virtual void Update(double deltaTime);
	virtual bool isFinished(void);


private:
	int m_uniqueID;
	cMeshObject* objToMove;

	glm::vec3 initPosition;
	glm::vec3 finalPosition;
	glm::vec3 direction;
	float distanceToTarget;
	float Speed;
	double initialTime;
	double elapsedTime;
	float Time;

	float AccRate;
	float DecRate;

	bool b_first;
	bool m_bIsDone;

	cMeshObject* targetGO;
};

