#ifndef _cMoveToCommand_HG_
#define _cMoveToCommand_HG_

#include "iCommand.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>


class cMoveToCommand : public iCommand
{
public:
	cMoveToCommand() { this->m_bIsDone = false; };

	virtual void Initialize(std::vector<sNVPair> vecNVPairs);
	virtual void Update(double deltaTime);
	virtual bool isFinished(void);


private:
	cMeshObject* objToMove;
	glm::vec3 destination;
	glm::vec3 start;
	glm::vec3 currentLocation;
	float minDistance = 20.0f;
	float speed;

	float time;	
	bool m_bIsDone;
};

#endif