#ifndef _cAnimationCurveCommand_HG_
#define _cAnimationCurveCommand_HG_

#include "iCommand.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>	// Note strange folder
#include <vector>


class cAnimationCurveCommand : public iCommand
{
public:
	virtual ~cAnimationCurveCommand() {};

	// Since this is an interface, this initialize would have to 
	//	be common for all the commands... 
	virtual void Initialize(std::vector<sNVPair> vecNVPairs);

	// These are both called every frame 
	virtual void Update(double deltaTime);
	virtual bool isFinished(void);

	// I am reading this every frame
	glm::vec3 currentLocation;

	glm::quat qCurrentOrientation;


private:

	void m_Update_Position(double deltaTime);
	void m_Update_Orientation(double deltaTime);

	// I pass a number between 0 and 1 and return a scaled value
	// between the min and max...
	float m_LERP(float minValue, float maxValue, float amountDone_0_to_1);

	bool m_bIsDone;

	float m_frameRate;	// Frames per second (FPS)

	float m_frameTime;	// Seconds between animation steps

	// How far into THIS frame am I?
	float m_currentFrameTime;


	unsigned int m_frameNumber;	// Start at 0 

	// Animation steps we will play back
	std::vector< glm::vec3 > m_vecPositionSteps;
	std::vector< glm::vec3 > m_vecOrientationSteps;
	// You could have any number of sets of these things
	//std::vector< glm:vec3 > m_vecOrientationSteps;
	//std::vector< glm:vec3 > m_vecColourSteps;



};

#endif