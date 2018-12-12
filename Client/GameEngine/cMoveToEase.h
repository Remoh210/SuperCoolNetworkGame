#ifndef _cMoveToEase_HG_
#define _cMoveToEase_HG_

#include "iCommand.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>


class cMoveToEase : public iCommand
{
public:
	cMoveToEase() { this->m_bIsDone = false; };

	virtual void Initialize(std::vector<sNVPair> vecNVPairs);
	virtual void Update(double deltaTime);
	virtual bool isFinished(void);
	glm::vec3 m_adjustVelocity(glm::vec3 vVel);

private:
	cMeshObject* objToMove;
	glm::vec3 destination;
	glm::vec3 start;
	glm::vec3 currentLocation;

	//Where is it Okay to finish c
	float minDistance = 1.0f;

	//When start to slow down & speed up in %
	float slowDownPer;
	float speed;

	float time;
	bool m_bIsDone;
};

#endif