#include "cFollowObjectCommand.h"

#include <iostream>

void cFollowObjectCommand::Initialize( std::vector<sNVPair> vecNVPairs )
{
	// Pull the data out of the vector
	this->idealCameraLocation = vecNVPairs[0].v3Value;	// idealCameraLocation
	this->minDistance = vecNVPairs[1].fValue;			// minDistance;	
	this->maxSpeedDistance = vecNVPairs[2].fValue;		// maxSpeedDistance
	this->maxSpeed = vecNVPairs[3].fValue;				// maxSpeed
	//
	this->currentLocation = vecNVPairs[4].v3Value;

	return;
}

void cFollowObjectCommand::Update( double deltaTime )
{
	// Get the information from the object that is moving
	float currentDistance = glm::distance( this->idealCameraLocation, this->currentLocation );

	if ( currentDistance <= this->minDistance )
	{
		// Yes, like Lady Gaga, I have arrived...
		this->m_bIsDone = true;
		return;
	}

	// Here, I am NOT where I'm supposed to be, so I need to move.
	// Calculate the direction vector to the target location
	glm::vec3 vDirection = this->idealCameraLocation - this->currentLocation;

	// To calc velocity, I will normalize
	glm::vec3 vVel = glm::normalize( vDirection );


	// Adjust velocity in the "slow down" zone
//	vVel = this->m_adjustVelocity( vVel );
	vVel = this->m_adjustVelocity_2( vVel );


	// How far to move in THIS time step?
	glm::vec3 deltaMove = vVel * (float)deltaTime;

	currentLocation += deltaMove;

	return;
}

glm::vec3 cFollowObjectCommand::m_adjustVelocity( glm::vec3 vVel )
{
	// Scale this with the speed we want to go...

	float currentDistance = glm::distance( this->idealCameraLocation, this->currentLocation );

	// At this point the MAX speed (and direction) is calculated.
	// Smooth step part....

	// Are we outside the "maximum speed zone"?
	if ( currentDistance >= this->maxSpeedDistance )
	{
		// Stay at max speed (redundant...)
		vVel = vVel * this->maxSpeed;
//		std::cout << "Going at max speed" << std::endl;
	}
	else if ( currentDistance <= this->minDistance )
	{
		// Stop the camera 
		vVel = glm::vec3(0.0f,0.0f,0.0f);
//		std::cout << "Going at min speed" << std::endl;
	}
	else
	{
		// I'm in the "in between spot" (I'm "slowing down...)

		// Smoothstep (and most of these function) need a value from 0 to 1
		float lengthOfSlowDownArea = this->maxSpeedDistance - this->minDistance;
		// 50.0 long

		float howFarFromMinDistance = currentDistance - this->minDistance;
		// 0.0 away from the "black circle" (speed = 0)

		float HowFarIntoSlowDownZone_0_to_1 = howFarFromMinDistance / lengthOfSlowDownArea;

		float HowFarIntoSlowDownZone_1_to_0 = 1.0f - HowFarIntoSlowDownZone_0_to_1;

		// Ramp function:
//		float slowDownRatio = HowFarIntoSlowDownZone_0_to_1;

//		// Smooth step function:
		float slowDownRatio = smoothstep_wikipedia( 1.0f, 0.0f, HowFarIntoSlowDownZone_1_to_0 );

		// glm::smoothstep takes a vec3....
 //		float slowDownRatio = glm::smoothstep( this->maxSpeed,  minSpeed, howFarIntoSlowDownZone_0_to_1 );

//		std::cout << "slowDownRatio 1: " << slowDownRatio << std::endl;

		// slowDownRatio is from 0.0 to 1.0
		vVel = vVel * (maxSpeed * slowDownRatio);
	}

	return vVel;
}

glm::vec3 cFollowObjectCommand::m_adjustVelocity_2( glm::vec3 vVel )
{
	float currentDistance = glm::distance( this->idealCameraLocation, this->currentLocation );

	float distance = clamp( this->minDistance, this->maxSpeedDistance, currentDistance );

	// Now distance will be MAX this->maxSpeedDistance...
	// Scale this from 0 to 1 (1 at max distance)
	float slowDownRatio = distance / (this->maxSpeedDistance - this->minDistance);

	// Smoothstep
	slowDownRatio = smoothstep_wikipedia( 0.0f, 1.0f, slowDownRatio );

//	std::cout << "slowDownRatio = " << slowDownRatio << std::endl;

	vVel *= (slowDownRatio * this->maxSpeed);

	return vVel;
}


bool cFollowObjectCommand::isFinished(void)
{
	return this->m_bIsDone;
}
