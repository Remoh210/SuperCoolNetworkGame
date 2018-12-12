#ifndef _LuaCGHelper_HG_
#define _LuaCGHelper_HG_

#include "iCommand.h"
#include "cCommandGroup.h"
#include "cMoveToEaseIO.h"


#include <string>

extern "C" {
#include <Lua5.3.3\lualib.h>
#include <Lua5.3.3\lua.h>
#include <Lua5.3.3\lauxlib.h>
}

class LuaCGHelper
{
public:
	LuaCGHelper();
	~LuaCGHelper();

	cMeshObject* findObjectByFriendlyName(std::string FriendlyName);
	static std::vector<cMeshObject*>* CGHelperObj;
	static int CreateGroup(lua_State *L); // <- C Lua Function

	static int CreateCommand(lua_State *L); // <- C Lua Function	

private:
	static int m_nextCommandID;
};

#endif
