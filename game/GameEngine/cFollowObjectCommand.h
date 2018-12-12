#ifndef _cFollowObjectCommand_HG_
#define _cFollowObjectCommand_HG_

#include "iCommand.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>


class cFollowObjectCommand : public iCommand
{
public:
	cFollowObjectCommand() { this->m_bIsDone = false; };

	virtual void Initialize(std::vector<sNVPair> vecNVPairs);
	virtual void Update(double deltaTime);
	virtual bool isFinished(void);


private:


	float minDistance;	// Any less than this and camera stops
	float maxSpeedDistance;	// If MORE than this far away...
	float maxSpeed;
	float time;

	glm::vec3 initPosition;
	glm::vec3 finalPosition;
	glm::vec3 idealPosition;
	glm::vec3 idealRelPosition;

	
	cMeshObject* theObj;
	cMeshObject* targetObj;

	bool b_Started;
	bool m_bIsDone;

	double initialTime;
	double elapsedTime;

};

#endif