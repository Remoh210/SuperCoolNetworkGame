#ifndef _cWait_HG_
#define _cWait_HG_

#include "iCommand.h"

class cWait : public iCommand
{
public:
	cWait();
	virtual ~cWait() {};

	virtual void Initialize(std::vector<sNVPair> vecNVPairs);

	// These are both called every frame 
	virtual void Update(double deltaTime);
	virtual bool isFinished(void);

private:

	float m_timeWaitedSoFar;
	float m_timeToWait;
};


#endif
