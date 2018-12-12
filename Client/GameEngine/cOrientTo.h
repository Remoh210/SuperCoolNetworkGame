#ifndef _cOrientTo_HG_
#define _cOrientTo_HG_
#include "iCommand.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

class cOrientTo : public iCommand
{
public:
	cOrientTo();
	~cOrientTo();

	virtual void Initialize(std::vector<sNVPair> vecNVPairs);
	virtual void Update(double deltaTime);
	virtual bool isFinished(void);



private:
	cMeshObject* theObj;

	double initialTime;
	double elapsedTime;
	float time;

	glm::vec3 targetPoint;
	glm::vec3 initPosition;
	glm::vec3 Up;
	glm::vec3 lookDirection;
	glm::quat initOrientation;
	glm::quat finalOrientation;


	bool b_Started;
	bool b_IsDone;

	cMeshObject* targetObj;
};

#endif