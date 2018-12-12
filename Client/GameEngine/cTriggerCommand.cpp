#include "cTriggerCommand.h"
#include <iostream>
#include <sstream>
#include <fstream>     
#include "globalStuff.h"






cTriggerCommand::cTriggerCommand()
{
	this->initialTime = 0;
	this->b_Started = false;
	this->m_bIsDone = false;
}


void cTriggerCommand::Initialize(std::vector<sNVPair> vecNVPairs)
{
	this->theObj = vecNVPairs[0].pMeshObj;
	this->triggerPoint = vecNVPairs[1].v3Value;
	this->radius = vecNVPairs[2].fValue;
	this->targetObj = vecNVPairs[3].pMeshObj;
	this->trigScript = vecNVPairs[4].sValue;
	if (this->targetObj != nullptr) { this->triggerPoint = this->targetObj->position; }

}


void cTriggerCommand::createTrigGroup(std::string trigScript)
{

	p_LuaScripts->LoadScriptFile(trigScript);

}

void cTriggerCommand::Update(double deltaTime)
{
	if (!this->b_Started)
	{
		this->distanceToTarget = glm::distance(this->triggerPoint, this->theObj->position);
		if (distanceToTarget <= this->radius)
		{
			this->b_Started = true;

		
			std::string scriptNameWithPath = "assets/scripts/" + this->trigScript;

			std::ifstream f(scriptNameWithPath);
			std::string strScript;
			if (f)
			{
				std::getline(f, strScript, '\0');
				std::cout << "Script " << scriptNameWithPath << " is loaded" << std::endl;
				std::cout << strScript << std::endl;
				::p_LuaScripts->RunThis(strScript);
			}
			else
			{
				std::cout << "Script " << scriptNameWithPath << " not found" << std::endl;
			}
			

			//::p_LuaScripts->Update(deltaTime);
			//::p_LuaScripts->UpdateCG(deltaTime);
			std::cout << this->trigScript << std::endl;

			this->m_bIsDone= true;
		}
	}
	
}


bool cTriggerCommand::isFinished()
{
	if (this->m_bIsDone) 
	{ 
		return true;
	}
	else 
	{ 
		return false; 
	}

	
	
}

cTriggerCommand::~cTriggerCommand()
{
	return;
}

