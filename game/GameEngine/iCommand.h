#ifndef _iCommand_HG_
#define _iCommand_HG_

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "cMeshObject.h"
#include "sNVPair.h"
#include "SmoothStep.h"

//struct sNVPair
//{
//	cMeshObject* pMeshObj;
//	std::string name;
//	std::string sValue;
//	float fValue;
//	int iValue;
//	glm::vec3 v3Value;
//};

class iCommand
{
public:
	// Since this is an interface, this initialize would have to 
	//	be common for all the commands... 
	virtual void Initialize(std::vector<sNVPair> vecNVPairs) = 0;

	// These are both called every frame 
	virtual void Update(double deltaTime) = 0;
	virtual bool isFinished(void) = 0;

	// WATCH THIS, or you get a memory leak
	virtual ~iCommand() {};		// empty constructor

};


#endif