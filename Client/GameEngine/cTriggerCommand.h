#pragma once
#include "iCommand.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

class cTriggerCommand : public iCommand
{
public:
	cTriggerCommand();
	~cTriggerCommand();

	virtual void Initialize(std::vector<sNVPair> vecNVPairs);
	virtual void Update(double deltaTime);
	virtual bool isFinished(void);
	void createTrigGroup(std::string trigScript);

private:
	cMeshObject* theObj;
	cMeshObject* targetObj;
	glm::vec3 triggerPoint;
	float distanceToTarget;
	float radius;


	std::string scriptName;
	std::string trigScript;

	double initialTime;
	float time;

	bool b_Started;
	bool m_bIsDone;
};

