#ifndef _cCommandGroup_HG_
#define _cCommandGroup_HG_

#include <vector>
#include "iCommand.h"

class cCommandGroup
{
public:
	cCommandGroup(std::string theName);

	virtual void Update(double deltaTime);
	virtual bool isFinished(void);

	// These are run in parallel (all at the same time)
	std::vector<iCommand*> vecCommands;
	// These are run in serial (one at a time)
	std::vector<cCommandGroup*> vecCommandGroups;
	std::string groupName;

private:
	bool bIsFinished;
};


#endif