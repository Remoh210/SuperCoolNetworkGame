#include "LuaCGHelper.h"
#include "cLuaBrain.h"

int LuaCGHelper::m_nextCommandID = 1;


cMeshObject* LuaCGHelper::findObjectByFriendlyName(std::string friendlyname)
{
	for (std::vector<cMeshObject*>::iterator itGO = vec_LuaMeshObj->begin();
		itGO != LuaCGHelper::CGHelperObj->end(); itGO++)
	{
		if ((*itGO)->friendlyName == friendlyname)
		{	// Found it!
			return (*itGO);
		}
	}//for ( std::vector<cMeshObject*>::iterator itGO...
	// Didn't find it
	return nullptr;
}
