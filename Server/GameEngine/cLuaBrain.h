#ifndef _cLuaBrain_HG_
#define _cLuaBrain_HG_

extern "C" {
#include <Lua5.3.3\lua.h>
#include <Lua5.3.3\lua.h>
#include <Lua5.3.3\lauxlib.h>
#include <Lua5.3.3/lualib.h>
}

#include <string>
#include <vector>
#include "cMeshObject.h"
#include <map>
#include "cCommandGroup.h"



class cLuaBrain
{
public:
	// Init Lua and set callback functions
	cLuaBrain();
	~cLuaBrain();
	void RunThis(std::string theLuaScript);
	void LoadScript( std::string scriptName, 
					 std::string scriptSource );
	void DeleteScript( std::string scriptName );
	void LoadScriptFile(std::string scriptName);
	void SetObjectVector( std::vector< cMeshObject* >* p_vecGOs );
	void Update(float deltaTime);
	void UpdateCG(float deltaTime);

	//Command Groups
	static int l_newCG(lua_State *L);
	static int l_newSubCG(lua_State *L);
	static int l_newCom(lua_State *L);

	//Commands
   // static int l_MoveObjEaseInOut(lua_State *L);
	//static int l_KillAllHumans( lua_State *L );	
	static int l_GetObjectState(lua_State *L);
	static int l_UpdateObject( lua_State *L );		// c function
	

	

	cMeshObject* pMyGameObject;

private:
	std::map< std::string /*scriptName*/, 
			  std::string /*scriptSource*/ > m_mapScripts;

	static std::vector< cMeshObject* >* m_p_vecGOs;
	// returns nullptr if not found
	static cMeshObject* m_findObjectByFriendlyName(std::string friendlyname);
	static cCommandGroup* m_findCGbyName(std::string groupName, cCommandGroup lua_CG);
	static cCommandGroup* m_findSubCGbyName(std::string subGroupName, cCommandGroup lua_CG);
	

	lua_State* m_pLuaState;

	std::string m_decodeLuaErrorToString( int error );
};

#endif

