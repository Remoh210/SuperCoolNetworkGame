#include "cLuaBrain.h"
#include "cFollowObjectCommand.h"
#include "cMoveToCommand.h"
#include "cMoveToEaseIO.h"
#include "cFollowCurve.h"
#include "cOrientTo.h"
#include "cTriggerCommand.h"
#include "Camera.h"
#include <iostream>
#include <fstream>



cCommandGroup luaCommandGroup("LuaMainGroup");


cLuaBrain::cLuaBrain()
{
	this->m_p_vecGOs = nullptr;

	this->m_pLuaState = luaL_newstate();

	luaL_openlibs(this->m_pLuaState);					

	//All in One command
	lua_pushcfunction(this->m_pLuaState, cLuaBrain::l_newSubCG);
	lua_setglobal(this->m_pLuaState, "newSubCG");

	//All in One command
	lua_pushcfunction(this->m_pLuaState, cLuaBrain::l_newCG);
	lua_setglobal(this->m_pLuaState, "newCG");




	lua_pushcfunction(this->m_pLuaState, cLuaBrain::l_newCom);
	lua_setglobal(this->m_pLuaState, "newCom");



	lua_pushcfunction(this->m_pLuaState, cLuaBrain::l_UpdateObject);
	lua_setglobal(this->m_pLuaState, "setObjectState");



	

	lua_pushcfunction(this->m_pLuaState, cLuaBrain::l_GetObjectState);
	lua_setglobal(this->m_pLuaState, "getObjectState");


	return;
}

cLuaBrain::~cLuaBrain()
{
	lua_close(this->m_pLuaState);
	return;
}


// Saves (and overwrites) any script
// scriptName is just so we can delete them later
void cLuaBrain::LoadScript(std::string scriptName,
	std::string scriptSource)
{
	this->m_mapScripts[scriptName] = scriptSource;
	return;
}


void cLuaBrain::DeleteScript(std::string scriptName)
{
	// TODO: delete this scritp
	return;
}

// Passes a pointer to the game object vector
void cLuaBrain::SetObjectVector(std::vector< cMeshObject* >* p_vecGOs)
{
	this->m_p_vecGOs = p_vecGOs;
	return;
}

void cLuaBrain::RunThis(std::string theLuaScript)
{
	int error = luaL_loadstring(this->m_pLuaState,
		theLuaScript.c_str());

	if (error != 0 /*no error*/)
	{
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << this->m_decodeLuaErrorToString(error) << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
	}

	// execute funtion in "protected mode", where problems are 
	//  caught and placed on the stack for investigation
	error = lua_pcall(this->m_pLuaState,	/* lua state */
		0,	/* nargs: number of arguments pushed onto the lua stack */
		0,	/* nresults: number of results that should be on stack at end*/
		0);	/* errfunc: location, in stack, of error function.
			  if 0, results are on top of stack. */
	if (error != 0 /*no error*/)
	{
		std::cout << "Lua: There was an error..." << std::endl;
		std::cout << this->m_decodeLuaErrorToString(error) << std::endl;

		std::string luaError;
		// Get error information from top of stack (-1 is top)
		luaError.append(lua_tostring(this->m_pLuaState, -1));

		// Make error message a little more clear
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << luaError << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
		// We passed zero (0) as errfunc, so error is on stack)
		lua_pop(this->m_pLuaState, 1);  /* pop error message from the stack */

	}

	return;
}




// Call all the active scripts that are loaded
void cLuaBrain::Update(float deltaTime)
{
	//	std::cout << "cLuaBrain::Update() called" << std::endl;
	for (std::map< std::string /*name*/, std::string /*source*/>::iterator itScript =
		this->m_mapScripts.begin(); itScript != this->m_mapScripts.end(); itScript++)
	{

		// Pass the script into Lua and update
//		int error = luaL_loadbuffer(L, buffer, strlen(buffer), "line");

		std::string curLuaScript = itScript->second;

		int error = luaL_loadstring(this->m_pLuaState,
			curLuaScript.c_str());

		if (error != 0 /*no error*/)
		{
			std::cout << "-------------------------------------------------------" << std::endl;
			std::cout << "Error running Lua script: ";
			std::cout << itScript->first << std::endl;
			std::cout << this->m_decodeLuaErrorToString(error) << std::endl;
			std::cout << "-------------------------------------------------------" << std::endl;
			continue;
		}

		// execute funtion in "protected mode", where problems are 
		//  caught and placed on the stack for investigation
		error = lua_pcall(this->m_pLuaState,	/* lua state */
			0,	/* nargs: number of arguments pushed onto the lua stack */
			0,	/* nresults: number of results that should be on stack at end*/
			0);	/* errfunc: location, in stack, of error function.
					 if 0, results are on top of stack. */
		if (error != 0 /*no error*/)
		{
			std::cout << "Lua: There was an error..." << std::endl;
			std::cout << this->m_decodeLuaErrorToString(error) << std::endl;

			std::string luaError;
			// Get error information from top of stack (-1 is top)
			luaError.append(lua_tostring(this->m_pLuaState, -1));

			// Make error message a little more clear
			std::cout << "-------------------------------------------------------" << std::endl;
			std::cout << "Error running Lua script: ";
			std::cout << itScript->first << std::endl;
			std::cout << luaError << std::endl;
			std::cout << "-------------------------------------------------------" << std::endl;
			// We passed zero (0) as errfunc, so error is on stack)
			lua_pop(this->m_pLuaState, 1);  /* pop error message from the stack */

			continue;
		}

	}

	// TODO: Lots of Lua stuff here...
	return;
}


/*static*/ int cLuaBrain::l_UpdateObject(lua_State *L)
{
	std::string objectFriendlyName = lua_tostring(L, 1);	/* get argument */

	// Exist? 
	cMeshObject* pGO = cLuaBrain::m_findObjectByFriendlyName(objectFriendlyName);

	if (pGO == nullptr)
	{	// No, it's invalid
		lua_pushboolean(L, false);
		// I pushed 1 thing on stack, so return 1;
		return 1;
	}

	// Object ID is valid
	// Get the values that lua pushed and update object
	pGO->position.x = lua_tonumber(L, 2);	/* get argument */
	pGO->position.y = lua_tonumber(L, 3);	/* get argument */
	pGO->position.z = lua_tonumber(L, 4);	/* get argument */
	pGO->velocity.x = lua_tonumber(L, 5);	/* get argument */
	pGO->velocity.y = lua_tonumber(L, 6);	/* get argument */
	pGO->velocity.z = lua_tonumber(L, 7);	/* get argument */

	lua_pushboolean(L, true);	// index is OK

	return 1;		// I'm returning ONE thing

}


int cLuaBrain::l_GetObjectState(lua_State *L)
{
	std::string objectFriendlyName = lua_tostring(L, 1);	/* get argument */

	// Exist? 
	cMeshObject* pGO = cLuaBrain::m_findObjectByFriendlyName(objectFriendlyName);

	if (pGO == nullptr)
	{	// No, it's invalid
		lua_pushboolean(L, false);
		// I pushed 1 thing on stack, so return 1;
		return 1;
	}

	// Object ID is valid
	lua_pushboolean(L, true);	// index is OK
	lua_pushnumber(L, pGO->position.x);
	lua_pushnumber(L, pGO->position.y);
	lua_pushnumber(L, pGO->position.z);
	lua_pushnumber(L, pGO->velocity.x);
	lua_pushnumber(L, pGO->velocity.y);
	lua_pushnumber(L, pGO->velocity.z);

	return 7;		// There were 7 things on the stack
}

/*static*/
std::vector< cMeshObject* >* cLuaBrain::m_p_vecGOs;


// returns nullptr if not found
/*static*/ cMeshObject* cLuaBrain::m_findObjectByFriendlyName(std::string friendlyname)
{
	for (std::vector<cMeshObject*>::iterator itGO = cLuaBrain::m_p_vecGOs->begin();
		itGO != cLuaBrain::m_p_vecGOs->end(); itGO++)
	{
		if ((*itGO)->friendlyName == friendlyname)
		{	// Found it!
			return (*itGO);
		}
	}//for ( std::vector<cMeshObject*>::iterator itGO...
	// Didn't find it
	return nullptr;
}


std::string cLuaBrain::m_decodeLuaErrorToString(int error)
{
	switch (error)
	{
	case 0:
		return "Lua: no error";
		break;
	case LUA_ERRSYNTAX:
		return "Lua: syntax error";
		break;
	case LUA_ERRMEM:
		return "Lua: memory allocation error";
		break;
	case LUA_ERRRUN:
		return "Lua: Runtime error";
		break;
	case LUA_ERRERR:
		return "Lua: Error while running the error handler function";
		break;
	}//switch ( error )

	// Who knows what this error is?
	return "Lua: UNKNOWN error";
}



int cLuaBrain::l_newCG(lua_State *L)
{
	std::string groupName = lua_tostring(L, 1);	// The name of the group

	cCommandGroup* NewGroup = new cCommandGroup(groupName);

	luaCommandGroup.vecCommandGroups.push_back(NewGroup);

	lua_pushboolean(L, true);
	return 1;
}




int cLuaBrain::l_newSubCG(lua_State *L)
{
	std::string groupName = lua_tostring(L, 1);	// The name of the group
	std::string subGroupName = lua_tostring(L, 2);	// The name of the subgroup


	
	cCommandGroup* NewSubGroup = new cCommandGroup(subGroupName);
	cCommandGroup* commandGroup = cLuaBrain::m_findCGbyName(groupName, luaCommandGroup);
	commandGroup->vecCommandGroups.push_back(NewSubGroup);

	lua_pushboolean(L, true);
	return 1;
}






//Load Script File (from assets/script)
void cLuaBrain::LoadScriptFile(std::string scriptName) 
{ 
	std::string scriptNameWithPath;
	scriptNameWithPath = "assets/scripts/" + scriptName;
	
	std::ifstream f(scriptNameWithPath);
	std::string strScript;
	if (f) 
	{
		std::getline(f, strScript, '\0');
		std::cout << "Script " << scriptNameWithPath << " is loaded" << std::endl;
		std::cout << strScript << std::endl;
		LoadScript(scriptName, strScript);
	}
	else 
	{ 
		std::cout << "Script " << scriptNameWithPath << " not found" << std::endl;
	}
	


}

void cLuaBrain::UpdateCG(float deltaTime) { luaCommandGroup.Update(deltaTime); }


int cLuaBrain::l_newCom(lua_State *L)
{
	std::string groupName = lua_tostring(L, 1);	
	std::string commandName = lua_tostring(L, 2); 
	std::string ObjFriendlyName = lua_tostring(L, 3);
	float x = lua_tonumber(L, 4);				
	float y = lua_tonumber(L, 5);				
	float z = lua_tonumber(L, 6);				
	float time = lua_tonumber(L, 7);	
	float x1 = lua_tonumber(L, 8);				
	float y1 = lua_tonumber(L, 9);				
	float z1 = lua_tonumber(L, 10);					
	std::string targetFriendlyName = lua_tostring(L, 11);			
	std::string script = lua_tostring(L, 12);


	//cCommandGroup* commandGroup = cLuaBrain::m_findCGbyName(groupName, luaCommandGroup);
	cCommandGroup* commandGroup = cLuaBrain::m_findSubCGbyName(groupName, luaCommandGroup);
	if (commandGroup == nullptr) 
	{
		commandGroup = cLuaBrain::m_findCGbyName(groupName, luaCommandGroup);
	}
	cMeshObject* theObject;
	
	//CAMERA HACK
	if (ObjFriendlyName == "camera" || ObjFriendlyName == "Camera") 
	{
		cMeshObject* p_camObj = new cMeshObject();
		p_camObj->friendlyName = "cameraObj";
		p_camObj->position = camera.Position;
		theObject = p_camObj;
	}
	else
	{
		theObject = cLuaBrain::m_findObjectByFriendlyName(ObjFriendlyName);
	}

	

	cMeshObject* targetObj;
	if (targetFriendlyName != "")
		targetObj = cLuaBrain::m_findObjectByFriendlyName(targetFriendlyName);
	else
		targetObj = nullptr;

	
	if (commandName == "Follow")
	{
		cFollowObjectCommand* newCommand = new cFollowObjectCommand();


		std::vector<sNVPair> vecInitValues;

		sNVPair ObjectToMove;				ObjectToMove.pMeshObj = theObject;
		sNVPair IdealRelPos;				IdealRelPos.v3Value = glm::vec3(x, y, z);
		sNVPair minDistance;				minDistance.fValue = x1;
		sNVPair maxSpeedDistance;			maxSpeedDistance.fValue = y1;
		sNVPair maxSpeed;					maxSpeed.fValue = z1;
		sNVPair TargetObject;				TargetObject.pMeshObj = targetObj;
		sNVPair Time;						Time.fValue = time;

		vecInitValues.push_back(ObjectToMove);
		vecInitValues.push_back(IdealRelPos);
		vecInitValues.push_back(minDistance);
		vecInitValues.push_back(maxSpeedDistance);
		vecInitValues.push_back(maxSpeed);
		vecInitValues.push_back(TargetObject);
		vecInitValues.push_back(Time);

		newCommand->Initialize(vecInitValues);
		commandGroup->vecCommands.push_back(newCommand);
	}
	
	else if (commandName == "FollowCurve")
	{
		cFollowCurve* newCommand = new cFollowCurve();


		std::vector<sNVPair> vecInitValues;


		sNVPair ObjectToMove;             ObjectToMove.pMeshObj = theObject;
		sNVPair ControlPoint;			  ControlPoint.v3Value = glm::vec3(x, y, z);
		sNVPair Destination;              Destination.v3Value = glm::vec3(x1, y1, z1);
		sNVPair Time;					  Time.fValue = time;
		sNVPair TargetObj;				  TargetObj.pMeshObj = targetObj;


		vecInitValues.push_back(ObjectToMove);
		vecInitValues.push_back(ControlPoint);
		vecInitValues.push_back(Destination);
		vecInitValues.push_back(Time);
		vecInitValues.push_back(TargetObj);

		newCommand->Initialize(vecInitValues);
		commandGroup->vecCommands.push_back(newCommand);
	}
	else if (commandName == "MoveToEaseIO")
	{
		cMoveToEaseIO* newCommand = new cMoveToEaseIO();

		std::vector<sNVPair> vecInitValues;

		sNVPair ObjectToMove;			ObjectToMove.pMeshObj = theObject;
		sNVPair Destination;			Destination.v3Value = glm::vec3(x, y, z);
		sNVPair Time;					Time.fValue = time;
		sNVPair SlowDownIn;				SlowDownIn.fValue = x1;
		sNVPair SlowDownOut;			SlowDownOut.fValue = y1;
		sNVPair TargetObject;           TargetObject.pMeshObj = targetObj;

		vecInitValues.push_back(ObjectToMove);
		vecInitValues.push_back(Destination);
		vecInitValues.push_back(Time);
		vecInitValues.push_back(SlowDownIn);
		vecInitValues.push_back(SlowDownOut);
		vecInitValues.push_back(TargetObject);

		newCommand->Initialize(vecInitValues);
		commandGroup->vecCommands.push_back(newCommand);
	}
	else if (commandName == "OrientTo")
	{
		cOrientTo* newCommand = new cOrientTo();

		std::vector<sNVPair> vecInitValues;

		sNVPair ObjectToRotate;			ObjectToRotate.pMeshObj = theObject;
		sNVPair Time;					Time.fValue = time;
		sNVPair TargetObject;           TargetObject.pMeshObj = targetObj;
		sNVPair TargetPoint;			TargetPoint.v3Value = glm::vec3(x, y, z);

		vecInitValues.push_back(ObjectToRotate);
		vecInitValues.push_back(Time);
		vecInitValues.push_back(TargetObject);
		vecInitValues.push_back(TargetPoint);

		newCommand->Initialize(vecInitValues);

		commandGroup->vecCommands.push_back(newCommand);
	}
	else if (commandName == "Trigger")
	{
		cTriggerCommand* newCommand = new cTriggerCommand();

		std::vector<sNVPair> vecInitValues;

		sNVPair Object;					 Object.pMeshObj = theObject;
		sNVPair TriggerPoint;			 TriggerPoint.v3Value = glm::vec3(x, y, z);
		sNVPair Radius;					 Radius.fValue = time;
		sNVPair TargetObj;				 TargetObj.pMeshObj = targetObj;
		sNVPair TrigScript;				 TrigScript.sValue = script;

		vecInitValues.push_back(Object);
		vecInitValues.push_back(TriggerPoint);
		vecInitValues.push_back(Radius);
		vecInitValues.push_back(TargetObj);
		vecInitValues.push_back(TrigScript);

		newCommand->Initialize(vecInitValues);

		commandGroup->vecCommands.push_back(newCommand);
	}
	
	else	
	{		

	}


	return 1;
}


 cCommandGroup* cLuaBrain::m_findCGbyName(std::string groupName, cCommandGroup lua_CG) {

	 for (int i = 0; i != lua_CG.vecCommandGroups.size(); i++)
	 {
		 if (lua_CG.vecCommandGroups[i]->groupName == groupName)
		 {
			 return lua_CG.vecCommandGroups[i];
		 }
	 }
}


 cCommandGroup* cLuaBrain::m_findSubCGbyName(std::string subGroupName, cCommandGroup lua_CG) {

	 for (int i = 0; i != lua_CG.vecCommandGroups.size(); i++)
	 {
		 for (int j = 0; j != lua_CG.vecCommandGroups[i]->vecCommandGroups.size(); j++)
		 {
			 if (lua_CG.vecCommandGroups[i]->vecCommandGroups[j]->groupName == subGroupName)
			 {
				 return lua_CG.vecCommandGroups[i]->vecCommandGroups[j];
			 }
		 }
	 }
	 return nullptr;
 }