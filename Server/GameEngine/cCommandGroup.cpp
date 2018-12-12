#include "cCommandGroup.h"



cCommandGroup::cCommandGroup(std::string theName)
{
	this->bIsFinished = false;
	groupName = theName;
}

void cCommandGroup::Update(double deltaTime)
{
	// Assume everything is done...
	bool bAllCommandsAreFinished = true;
	bool bAllCommandGroupsAreFinished = true;

	// Run ALL the "commands" at the same time (in parallel)
	for (std::vector<iCommand*>::iterator itCurCom = this->vecCommands.begin();
		itCurCom != this->vecCommands.end(); itCurCom++)
	{
		iCommand* pCurCom = *itCurCom;

		if (!pCurCom->isFinished())
		{
			pCurCom->Update(deltaTime);

			// At least one command is still running...
			bAllCommandsAreFinished = false;
		}
	}

	// Find the 1st command that is NOT done and run that
	for (std::vector<cCommandGroup*>::iterator itCurComGrp = this->vecCommandGroups.begin();
		itCurComGrp != this->vecCommandGroups.end(); itCurComGrp++)
	{
		cCommandGroup* pCurCmdGrp = *itCurComGrp;

		// This one NOT running? 
		if (!pCurCmdGrp->isFinished())
		{
			// Yes, so update...
			pCurCmdGrp->Update(deltaTime);

			bAllCommandGroupsAreFinished = false;
			// ... and quit the loop
			break;
		}
	}

	if (bAllCommandsAreFinished && bAllCommandGroupsAreFinished)
	{
		this->bIsFinished = true;
	}

	// Run the command groups one at a time
}


bool cCommandGroup::cCommandGroup::isFinished(void)
{
	return this->bIsFinished;
}
