#include "cAnimationCurveCommand.h"

#include <iostream>

void cAnimationCurveCommand::Initialize(std::vector<sNVPair> vecNVPairs)
{
	// Normally, you'd pass the data into the command in some way, 
	// but for time, we'll just load the data here...
	// (like a motion capture file, or whatever... 
	//  ... so some giant list of numbers representing locations, orientation, etc.)
	const float HACK_ratio = 3.0f;

	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 0.0f, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 0.960, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 1.920, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 2.500, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 2.980, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 3.690, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 4.300, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 4.540, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 5.210, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 5.460, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 5.630, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 6.530, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 7.100, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 7.210, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 7.260, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 7.610, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 7.690, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 8.550, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 8.870, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 8.970, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 9.450, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 10.03, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 10.83, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 10.98, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 11.46, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 11.47, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 12.40, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 13.39, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 13.42, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 14.10, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 14.65, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 15.18, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 15.38, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 15.92, 0.0f, 0.0f));
	this->m_vecPositionSteps.push_back(glm::vec3(HACK_ratio * 16.72, 0.0f, 0.0f));

	float HACK_OrientationRatio = 20.0f;
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 0.0f, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 0.960, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 1.920, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 2.500, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 2.980, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 3.690, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 4.300, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 4.540, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 5.210, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 5.460, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 5.630, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 6.530, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 7.100, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 7.210, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 7.260, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 7.610, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 7.690, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 8.550, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 8.870, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 8.970, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 9.450, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 10.03, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 10.83, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 10.98, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 11.46, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 11.47, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 12.40, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 13.39, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 13.42, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 14.10, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 14.65, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 15.18, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 15.38, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 15.92, 0.0f, 0.0f));
	this->m_vecOrientationSteps.push_back(glm::vec3(HACK_OrientationRatio * 16.72, 0.0f, 0.0f));


	//	this->m_frameRate = 30.0f;		// 32 ms
	this->m_frameRate = 0.5f;		// 32 ms

	this->m_frameTime = 1.0f / this->m_frameRate;	// around 32 ms

	this->m_frameNumber = 0;			// Start at frame 0
	this->m_currentFrameTime = 0.0f;	// Start of animation (time = 0)

	// 
	this->currentLocation = this->m_vecPositionSteps[0];

	return;
}

// These are both called every frame 
// 0.001 seconds
// 0.033 ms frame time (at 30 FPS)
void cAnimationCurveCommand::Update(double deltaTime)
{
	if (this->m_bIsDone)
	{
		return;
	}

	// 0.001 .002 0.003   0.033?
	this->m_currentFrameTime += (float)deltaTime;

	if (this->m_currentFrameTime >= this->m_frameTime)
	{
		this->m_frameNumber++;

		// We could reset to zero, but there will alway be a "little bit of time" left
		//this->m_currentFrameTime = 0.0f;

		// Instead, we take away the frame time (33ms)
		// Leaving the "left over bit"
		this->m_currentFrameTime -= this->m_frameTime;
	}

	// Have I played the last frame? 
	if (this->m_frameNumber >= this->m_vecPositionSteps.size())
	{
		// Yes
		this->m_bIsDone = true;
		return;
	}

	this->m_Update_Position(deltaTime);
	this->m_Update_Orientation(deltaTime);


	return;
}

bool cAnimationCurveCommand::isFinished(void)
{
	return this->m_bIsDone;
}

// I pass a number between 0 and 1 and return a scaled value
// between the min and max...
float cAnimationCurveCommand::m_LERP(float minValue, float maxValue, float valueInBetween)
{
	// Or "interpolated" value
	float LEFRP_Value = ((maxValue - minValue) * valueInBetween) + minValue;

	return LEFRP_Value;
}

void cAnimationCurveCommand::m_Update_Position(double deltaTime)
{
	// At this point we have a valid frame.... 
// Look up the position (or whatever) information
//this->currentLocation = this->m_vecPositionSteps[this->m_frameNumber];

// Use LERP to see how far in between these values we are:
// What value was the LAST frame? 
	glm::vec3 thisFramePos = this->m_vecPositionSteps[this->m_frameNumber];

	// What value is the NEXT frame? 
	unsigned int nextFrameIndex = this->m_frameNumber + 1;
	// Are we off the end of our vector of animation frames? 
	if (nextFrameIndex >= this->m_vecPositionSteps.size())
	{
		nextFrameIndex = this->m_vecOrientationSteps.size() - 1;
	}
	glm::vec3 nextFramePos = this->m_vecPositionSteps[nextFrameIndex];


	// What's the "in between value" we WOULD have (if we had it)? 
	// Returns a value between 0 and 1
	float ratioIntoFrame = this->m_currentFrameTime / this->m_frameTime;
	if (ratioIntoFrame > 1.0f)
	{
		ratioIntoFrame = 1.0f;
	}

	this->currentLocation.x = this->m_LERP(thisFramePos.x, nextFramePos.x, this->m_currentFrameTime);
	this->currentLocation.y = this->m_LERP(thisFramePos.y, nextFramePos.y, this->m_currentFrameTime);
	this->currentLocation.z = this->m_LERP(thisFramePos.z, nextFramePos.z, this->m_currentFrameTime);


	std::cout << "Animating at frame: " << this->m_frameNumber
		<< "   " << this->m_frameTime << " seconds" << std::endl;

}

void cAnimationCurveCommand::m_Update_Orientation(double deltaTime)
{
	// Would do a SLERP here
	this->qCurrentOrientation = glm::quat(this->m_vecOrientationSteps[this->m_frameNumber]);

	return;
}
