#ifndef _globalStuff_HG_
#define _globalStuff_HG_


#include "globalOpenGLStuff.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "cAABB.h"

#include <string>

#include "sPlyVertex.h"
#include "sPlyTriangle.h"
#include "cMeshObject.h"
#include "sModelDrawInfo.h"
#include "cVAOMeshManager.h"
#include "cLuaBrain.h"
#include "cLightManager.h"

#include "Error/CErrorLog.h"
#include "TextureManager/cBasicTextureManager.h"
#include "DebugRenderer/iDebugRenderer.h"
// The commands
#include "cCommandGroup.h"
// And the commands
#include "cFollowObjectCommand.h"
#include "cMoveToCommand.h"
#include "cMoveToEase.h"
#include "cMoveToEaseIO.h"
// ... and more
extern GLuint program;

extern bool bMouseInWindow;
extern cBasicTextureManager* g_pTheTextureManager;

//extern cCommandGroup* luaCommandGroup;
extern cCommandGroup sceneCommandGroup;
extern std::vector<cAABB::sAABB_Triangle> vec_cur_AABB_tris;
//extern std::vector<cMeshObject*> vec_pObjectsToDraw;

extern glm::vec3 g_CameraEye;	// = glm::vec3( 0.0, 0.0, +10.0f );
//extern glm::vec3 g_CameraAt;	// = glm::vec3( 0.0, 0.0, 0.0f );
extern double deltaTime;
//Global screen size
extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

// NOTE: These are JUST FOR NOW!!!
extern glm::vec3 g_lightPos;	// = glm::vec3( 4.0f, 4.0f, 0.0f );
extern float g_lightBrightness;	// = 1.0f;

extern cVAOMeshManager* g_pTheVAOMeshManager;
extern std::vector< cMeshObject* > vec_pObjectsToDraw;

// 
// This is a global pointer to a mesh object. 
// You can get at object like this, or you can 
// search for them using the findObjectBy...() 
// functions.
extern cMeshObject* g_pRogerRabbit;
// 

// Signature for the ply loader function
bool LoadPlyFileData( std::string fileName );

void LoadModelTypes( cVAOMeshManager* pTheVAOMeshManager, GLuint shaderProgramID );
void CreateModels(std::string filename, cVAOMeshManager* pTheVAOMeshManager, GLuint shaderProgramID);
void LoadModelsIntoScene( std::vector<cMeshObject*> &vec_pObjectsToDraw );


void loadCameraInfo(std::string filename);
void saveCameraInfo(std::string filename);

cMeshObject* findObjectByFriendlyName(std::string theNameToFind);
cMeshObject* findObjectByUniqueID( unsigned int IDToFind );
extern cLuaBrain* p_LuaScripts;

void saveLightInfo(std::string filename, std::vector<sLight*> lights);

void saveModelInfo(std::string filename, std::vector<cMeshObject*> models);

void loadModels(std::string filename, std::vector<cMeshObject*> models);

void loadLights(std::string filename, std::vector<sLight*> lights);

void DrawObject( cMeshObject* pCurrentMesh,
				 glm::mat4x4 &matModel, 
				 GLuint shaderProgramID );


// *****************************************************************
// This is part of the physics stuff
// (really should be moved to the physics area, not here)
struct sClosestPointData
{
	glm::vec3 thePoint;
	unsigned int triangleIndex;
};
void CalculateClosestPointsOnMesh( sModelDrawInfo theMeshDrawInfo, 
								   glm::vec3 pointToTest, 
								   std::vector<sClosestPointData> &vecPoints );
// *****************************************************************


//https://stackoverflow.com/questions/9878965/rand-between-0-and-1
template <class T>
T getRandBetween0and1(void)
{
	return (T) ((double) rand() / (RAND_MAX)) ;
}

// Inspired by: https://stackoverflow.com/questions/686353/c-random-float-number-generation
template <class T>
T getRandInRange( T min, T max )
{
	double value = 
		min + static_cast <double> (rand()) 
		/ ( static_cast <double> (RAND_MAX/(static_cast<double>(max-min) )));
	return static_cast<T>(value);
}

extern iDebugRenderer* g_pDebugRenderer;

#endif	// _globalStuff_HG_
