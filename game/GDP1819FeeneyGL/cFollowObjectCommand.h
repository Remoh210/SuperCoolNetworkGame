#ifndef _cFollowObjectCommand_HG_
#define _cFollowObjectCommand_HG_

#include "iCommand.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>


class cFollowObjectCommand : public iCommand
{
public:
	cFollowObjectCommand() { this->m_bIsDone = false; };

	// From iCommand interface
	virtual void Initialize( std::vector<sNVPair> vecNVPairs );
	virtual void Update( double deltaTime );
	virtual bool isFinished(void);

	// This is public so I can grab it for the camera or object
	glm::vec3 currentLocation;

private:
	glm::vec3 idealCameraLocation;

	float minDistance;	// Any less than this and camera stops

	float maxSpeedDistance;	// If MORE than this far away...
	// ... go at this maximum speed
	float maxSpeed;		

	bool m_bIsDone;

	glm::vec3 m_adjustVelocity( glm::vec3 vVel );
	glm::vec3 m_adjustVelocity_2( glm::vec3 vVel );

};

#endif
