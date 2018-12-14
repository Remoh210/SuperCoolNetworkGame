#define WIN32_LEAN_AND_MEAN
//     ___                 ___ _     
//    / _ \ _ __  ___ _ _ / __| |    
//   | (_) | '_ \/ -_) ' \ (_ | |__  
//    \___/| .__/\___|_||_\___|____| 
//         |_|                       
//
#include "globalOpenGLStuff.h"
#include "globalStuff.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include "Camera.h"
#include <stdlib.h>
#include <stdio.h>		// printf();
#include <iostream>		// cout (console out)
#include <vector>		// "smart array" dynamic array
#include "cShaderManager.h"
#include "cMeshObject.h"
#include "cVAOMeshManager.h"
#include <algorithm>
#include "DebugRenderer/cDebugRenderer.h"
#include "cLightHelper.h"

//**Network Stuff**//

#include <windows.h>
#include "networking/ConnectionMaintainer.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#include "Player.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")
#define BUFFER_LENGTH 512
#define PORT_NUMBER "5000"
#define SERVER_ADDRESS "127.0.0.1"
#define ARRAY_SIZE 128  

//**Network Stuff**//

GLuint program;
cDebugRenderer* g_pDebugRendererACTUAL = NULL;
iDebugRenderer* g_pDebugRenderer = NULL;
cLuaBrain* p_LuaScripts = NULL;
cCommandGroup sceneCommandGroup("SceneCG");








std::vector<cAABB::sAABB_Triangle> vec_cur_AABB_tris;
void UpdateWindowTitle(void);
double currentTime = 0;
double deltaTime = 0;

void sendInput();

void DoPhysicsUpdate( double deltaTime, 
					  std::vector< cMeshObject* > &vec_pObjectsToDraw );

std::vector< cMeshObject* > vec_pObjectsToDraw;
float str = 0;
// To the right, up 4.0 units, along the x axis
glm::vec3 g_lightPos = glm::vec3( 4.0f, 4.0f, 0.0f );
float g_lightBrightness = 400000.0f;

unsigned int numberOfObjectsToDraw = 0;

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

bool distToCam(cMeshObject* leftObj, cMeshObject* rightObj) {
	return glm::distance(leftObj->position, camera.Position) > glm::distance(rightObj->position, camera.Position); // here go your sort conditions
}

std::vector <cMeshObject*> vec_sorted_drawObj;

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 g_CameraEye = glm::vec3( 0.0, 0.0, 250.0f );

//glm::vec3 g_CameraAt = glm::vec3(g_CameraEye, g_CameraEye.z + cameraFront.z, cameraUp.y);
//glm::vec3 g_CameraAt = glm::vec3( 0.0, 0.0, 0.0f );


cShaderManager* pTheShaderManager = NULL;		// "Heap" variable
cVAOMeshManager* g_pTheVAOMeshManager = NULL;

cLightManager* LightManager = NULL;

std::vector<cMeshObject*> vec_transObj;
std::vector<cMeshObject*> vec_non_transObj;

float receiveDelay = 0.01f;
int counter = 0;

cBasicTextureManager* g_pTheTextureManager = NULL;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

cAABBHierarchy* g_pTheTerrain = new cAABBHierarchy();


void DoPhysicsCheckpointNumberFour(double deltaTime);

// For now, I'm doing this here, but you might want to do this
//  in the object, in the "phsyics" thing, or wherever. 
//  Or leave it here!!
void LoadTerrainAABB(void);


ConnectionMaintainer Conn;
UserInfo User;

Player players[4];

int main(void)
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Light", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}


	//CAMERA SPEED
	camera.MovementSpeed = 1000.0f;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	//glfwSetKeyCallback()
	

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);


	// Create the shader manager...
	//cShaderManager TheShaderManager;		// 
	//cShaderManager* pTheShaderManager;		// 
	pTheShaderManager = new cShaderManager();
	pTheShaderManager->setBasePath("assets/shaders/");

	cShaderManager::cShader vertexShader;
	cShaderManager::cShader fragmentShader;

	vertexShader.fileName = "vertex01.glsl";
	vertexShader.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	fragmentShader.fileName = "fragment01.glsl";
	fragmentShader.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if (pTheShaderManager->createProgramFromFile("BasicUberShader",
		vertexShader,
		fragmentShader))
	{		// Shaders are OK
		std::cout << "Compiled shaders OK." << std::endl;
	}
	else
	{
		std::cout << "OH NO! Compile error" << std::endl;
		std::cout << pTheShaderManager->getLastError() << std::endl;
	}


	// Load the uniform location values (some of them, anyway)
	cShaderManager::cShaderProgram* pSP = ::pTheShaderManager->pGetShaderProgramFromFriendlyName("BasicUberShader");
	pSP->LoadUniformLocation("texture00");
	pSP->LoadUniformLocation("texture01");
	pSP->LoadUniformLocation("texture02");
	pSP->LoadUniformLocation("texture03");
	pSP->LoadUniformLocation("texture04");
	pSP->LoadUniformLocation("texture05");
	pSP->LoadUniformLocation("texture06");
	pSP->LoadUniformLocation("texture07");
	pSP->LoadUniformLocation("texBlendWeights[0]");
	pSP->LoadUniformLocation("texBlendWeights[1]");




	program = pTheShaderManager->getIDFromFriendlyName("BasicUberShader");


	::g_pTheVAOMeshManager = new cVAOMeshManager();
	// Create the texture manager
	::g_pTheTextureManager = new cBasicTextureManager();




	// Loading the uniform variables here (rather than the inner draw loop)
	GLint objectColour_UniLoc = glGetUniformLocation(program, "objectColour");
	//uniform vec3 lightPos;
	//uniform float lightAtten;



	GLint matModel_location = glGetUniformLocation(program, "matModel");
	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");
	GLint eyeLocation_location = glGetUniformLocation(program, "eyeLocation");

	// Note that this point is to the +interface+ but we're creating the actual object
	::g_pDebugRendererACTUAL = new cDebugRenderer();
	::g_pDebugRenderer = (iDebugRenderer*)::g_pDebugRendererACTUAL;

	if (!::g_pDebugRendererACTUAL->initialize())
	{
		std::cout << "Warning: couldn't init the debug renderer." << std::endl;
		std::cout << "\t" << ::g_pDebugRendererACTUAL->getLastError() << std::endl;
	}
	else
	{
		std::cout << "Debug renderer is OK" << std::endl;
	}

	// Loading models was moved into this function
	LoadModelTypes(::g_pTheVAOMeshManager, program);
	CreateModels("Models.txt", ::g_pTheVAOMeshManager, program);
	LoadModelsIntoScene(::vec_pObjectsToDraw);

	//vec_sorted_drawObj = vec_pObjectsToDraw;

			// Draw all the objects in the "scene"
	for (unsigned int objIndex = 0;
		objIndex != (unsigned int)vec_pObjectsToDraw.size();
		objIndex++)
	{
		cMeshObject* pCurrentMesh = vec_pObjectsToDraw[objIndex];
		if (pCurrentMesh->materialDiffuse.a < 1.0f) { vec_transObj.push_back(pCurrentMesh); }
		else { vec_non_transObj.push_back(pCurrentMesh); }

	}//for ( unsigned int objIndex = 0; 


	LoadTerrainAABB();



	// Get the current time to start with
	double lastTime = glfwGetTime();


	//***************************************************************

	LightManager = new cLightManager();


	{
		sLight* pTheMainLight = new sLight();
		pTheMainLight->position = glm::vec4(1.0f, 400.0f, 0.0f, 1.0f);
		pTheMainLight->atten.x = 0.0f;	// 			float constAtten = 0.0f;
		pTheMainLight->atten.y = 0.0001f;	//			float linearAtten = 0.01f;
		pTheMainLight->atten.z = 0.00001f;	//			float quadAtten = 0.001f;
		pTheMainLight->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);// White light
		pTheMainLight->param2.x = 1.0f;
		pTheMainLight->SetLightType(sLight::POINT_LIGHT);
		pTheMainLight->SetSpotConeAngles(15.0f, 35.0f);
		//	pTheOneLight->SetSpotConeAngles( 15.0f, 45.0f );
			// Direction is RELATIVE to the LIGHT (for spots)
			// Straight down... 
		pTheMainLight->SetRelativeDirection(glm::vec3(0.0f, -1.0f, 1.0f));
		//pTheForthLight->AtenSphere - false;
		pTheMainLight->lightName = "MainLight";
		LightManager->vecLights.push_back(pTheMainLight);
		LightManager->LoadUniformLocations(program);
	}

	for(int light_count = 0; light_count < 4;  light_count++)
	{
		sLight* pTorch = new sLight();
		pTorch->position = glm::vec4(light_count * 10.0f, 400.0f, 0.0f, 1.0f);
		pTorch->atten.x = 0.0f;	
		pTorch->atten.y = 0.0001f;	
		pTorch->atten.z = 0.000015f;	
		pTorch->diffuse = glm::vec4(232 /250.0f, 109 / 250.0f, 27/250.0f, 1.0f);
		pTorch->SetLightType(sLight::SPOT_LIGHT);
		pTorch->SetSpotConeAngles(15.0f, 35.0f);
		pTorch->SetRelativeDirection(glm::vec3(0.0f, -1.0f, 1.0f));
		pTorch->lightName = "Torch_Light" + std::to_string(light_count);
		LightManager->vecLights.push_back(pTorch);
		LightManager->LoadUniformLocations(program);
	}

	{
		sLight* ChestLight = new sLight();
		ChestLight->position = glm::vec4(-675.0f, 40.0f, 520.0f, 1.0f);
		ChestLight->atten.x = 0.0f;	
		ChestLight->atten.y = 0.0001f;	
		ChestLight->atten.z = 0.000021f;	
		ChestLight->diffuse = glm::vec4(246 / 250.0f, 10/ 250.0f, 10 / 215.0f, 0.0f);
		ChestLight->param2.x = 0.0f;
		//pTheForthLight->AtenSphere - false;
		ChestLight->lightName = "ChestLight";
		LightManager->vecLights.push_back(ChestLight);
		LightManager->LoadUniformLocations(program);
	}


	{
		sLight* ChestLight = new sLight();
		ChestLight->position = glm::vec4(-675.0f, 40.0f, 520.0f, 1.0f);
		ChestLight->atten.x = 0.0f;	// 			float constAtten = 0.0f;
		ChestLight->atten.y = 0.0001f;	//			float linearAtten = 0.01f;
		ChestLight->atten.z = 0.000021f;	//			float quadAtten = 0.001f;
		ChestLight->diffuse = glm::vec4(0.64f, 0.027f, 0.9f, 1.0f);
		ChestLight->param2.x = 0.0f;
		ChestLight->lightName = "QuestLight";
		LightManager->vecLights.push_back(ChestLight);
		LightManager->LoadUniformLocations(program);
	}

	for (int light_count = 0; light_count < 8; light_count++)
	{
		sLight* pTorch = new sLight();
		pTorch->position = glm::vec4(light_count * 10.0f, 400.0f, 0.0f, 1.0f);
		pTorch->atten.x = 0.0f;
		pTorch->atten.y = 0.0001f;
		pTorch->atten.z = 0.000015f;
		pTorch->diffuse = glm::vec4(232 / 250.0f, 109 / 250.0f, 27 / 250.0f, 1.0f);
		pTorch->SetLightType(sLight::SPOT_LIGHT);
		pTorch->SetSpotConeAngles(15.0f, 35.0f);
		pTorch->SetRelativeDirection(glm::vec3(0.0f, -1.0f, 1.0f));
		pTorch->lightName = "SpaceShipLight_" + std::to_string(light_count);
		LightManager->vecLights.push_back(pTorch);
		LightManager->LoadUniformLocations(program);
	}

	cLightHelper* pLightHelper = new cLightHelper();

	


	loadLights("lights.txt", LightManager->vecLights);
	loadCameraInfo("camera.txt");

	




	//::p_LuaScripts = new cLuaBrain();
	//::p_LuaScripts->SetObjectVector(&(::vec_pObjectsToDraw));

	//::p_LuaScripts->LoadScriptFile("example.lua");
	camera.b_controlledByScript = true;
	//COMMANDS
	cFollowObjectCommand* newCommand = new cFollowObjectCommand();

	cMeshObject* p_camObj = new cMeshObject();
	p_camObj->friendlyName = "cameraObj";
	p_camObj->position = camera.Position;

	std::vector<sNVPair> vecInitValues;

	sNVPair ObjectToMove;				ObjectToMove.pMeshObj = p_camObj;
	sNVPair IdealRelPos;				IdealRelPos.v3Value = glm::vec3(0.0f, 1.0f, 0.0f);
	sNVPair minDistance;				minDistance.fValue = 4;
	sNVPair maxSpeedDistance;			maxSpeedDistance.fValue = 20;
	sNVPair maxSpeed;					maxSpeed.fValue = 30;
	sNVPair TargetObject;				TargetObject.pMeshObj = findObjectByFriendlyName("car");
	sNVPair Time;						Time.fValue = 0;

	vecInitValues.push_back(ObjectToMove);
	vecInitValues.push_back(IdealRelPos);
	vecInitValues.push_back(minDistance);
	vecInitValues.push_back(maxSpeedDistance);
	vecInitValues.push_back(maxSpeed);
	vecInitValues.push_back(TargetObject);
	vecInitValues.push_back(Time);

	newCommand->Initialize(vecInitValues);
	sceneCommandGroup.vecCommands.push_back(newCommand);



	//*****************************************************************


	std::cout << " \n\
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl << "\
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl << "\
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl << "\
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl << "\
::::::::::::::::##############                              :::::::::::::::::::" << std::endl << "\
############################  ##############################  :::::::::::::::::" << std::endl << "\
#########################  ######???????????????????????######  :::::::::::::::" << std::endl << "\
=========================  ####??????????()????()?????????####  :::::::::::::::" << std::endl << "\
=========================  ##????()??????????????    ()?????##  ::::    :::::::" << std::endl << "\
------------=============  ##??????????????????  ;;;;  ?????##  ::  ;;;;  :::::" << std::endl << "\
-------------------------  ##??????????()??????  ;;;;;;?????##    ;;;;;;  :::::" << std::endl << "\
-------------------------  ##??????????????????  ;;;;;;         ;;;;;;;;  :::::" << std::endl << "\
++++++++++++-------------  ##??????????????????  ;;;;;;;;;;;;;;;;;;;;;;;  :::::" << std::endl << "\
+++++++++++++++++++++++++  ##????????????()??  ;;;;;;;;;;;;;;;;;;;;;;;;;;;  :::" << std::endl << "\
+++++++++++++++++    ;;;;  ##??()????????????  ;;;;;;@@  ;;;;;;;;@@  ;;;;;  :::" << std::endl << "\
~~~~~~~~~~~~~++++;;;;;;;;  ##????????????????  ;;;;;;    ;;;  ;;;    ;;;;;  :::" << std::endl << "\
~~~~~~~~~~~~~~~  ;;  ~~~~  ####??????()??????  ;;[];;;;;;;;;;;;;;;;;;;;;[]  :::" << std::endl << "\
$$$$$$$$$$$$$~~~~  ~~~~~~  ######?????????????  ;;;;;;              ;;;;  :::::" << std::endl << "\
$$$$$$$$$$$$$$$$$$$$$$$$$    ###################  ;;;;;;;;;;;;;;;;;;;;  :::::::" << std::endl << "\
$$$$$$$$$$$$$$$$$$$$$$$  ;;;;                                       :::::::::::" << std::endl << "\
:::::::::::::$$$$$$$$$$  ;;;;  ::  ;;  ::::::::::::  ;;  ::  ;;;;  ::::::::::::" << std::endl << "\
:::::::::::::::::::::::      ::::::    :::::::::::::     ::::      ::::::::::::" << std::endl << "\
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl << "\
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl << "\
::::::::::::::::NN::::NN::YY::::YY:::AAAAAA:::NN::::NN:::!!::::::::::::::::::::" << std::endl << "\
::::::::::::::::NNNN::NN::YY::::YY::AA::::AA::NNNN::NN:::!!::::::::::::::::::::" << std::endl << "\
::::::::::::::::NNNN::NN::YY::::YY::AA::::AA::NNNN::NN:::!!::::::::::::::::::::" << std::endl << "\
::::::::::::::::NN::NNNN::::YYYY::::AAAAAAAA::NN::NNNN:::!!::::::::::::::::::::" << std::endl << "\
::::::::::::::::NN::NNNN:::::YY:::::AA::::AA::NN::NNNN:::::::::::::::::::::::::" << std::endl << "\
::::::::::::::::NN::::NN:::::YY:::::AA::::AA::NN::::NN:::!!::::::::::::::::::::" << std::endl << "\
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl << "\
::::::::::::::::::::::::YOU:HAVE:DONE:THE:NYAN:::::::::::::::::::::::::::::::::" << std::endl;
	std::cout << "Welcome to The Super Cool Racing game";
	std::cout << "Connecting to the game server\n";

	User.ServerAdress = SERVER_ADDRESS;


	std::cout << "Enter your nick name: \n";
	char FirstName[ARRAY_SIZE];
	std::cin >> FirstName;
	User.Name = FirstName;

	// Buffer send
	Buffer* Send_Buffer = new Buffer(BUFFER_LENGTH);

	// Buffer recieve
	Buffer* Recieve_Buffer = new Buffer(BUFFER_LENGTH);

	
	Conn.Connect(User);


	if (Conn.isAlive) {
		cout << "You appear to be connected!\n";
		//Sleep(3000);
	}
	else {
		cout << "Looks like the server is down, no luck this time.\n";
		Sleep(3000);
	}


	string Send_Message;
	string Recieve_Message;
	string ChatBuffer;
	vector<string> ConnRooms;

	if (Conn.isAlive) {

		std::cout << "Enter Room Name \n";
		char room[ARRAY_SIZE];
		std::cin >> room;
		User.Room = room;
		//char* RoomName;
		//RoomName = new char[ARRAY_SIZE];
		//string RoomName = "main";
		//User.Room = (char*)RoomName.c_str();
		ConnRooms.push_back(User.Room);
		Conn.sendMessage(Send_Buffer, User, MSG_ID_JOIN_ROOM, 0, Send_Message);
	}


	
	// Draw the "scene" (run the program)
	while (!glfwWindowShouldClose(window))
    {



		// Switch to the shader we want
		::pTheShaderManager->useShaderProgram( "BasicUberShader" );

        float ratio;
        int width, height;



		glm::mat4x4 matProjection = glm::mat4(1.0f);
		glm::mat4x4	matView = glm::mat4(1.0f);
 

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);


		glEnable( GL_DEPTH );		// Enables the KEEPING of the depth information
		glEnable( GL_DEPTH_TEST );	// When drawing, checked the existing depth
		glEnable( GL_CULL_FACE );	// Discared "back facing" triangles

		// Colour and depth buffers are TWO DIFF THINGS.
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		//mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		matProjection = glm::perspective( 1.0f,			// FOV
			                                ratio,		// Aspect ratio
			                                0.1f,			// Near clipping plane
			                                25000.0f );	// Far clipping plane


		//glm::vec3 migpos = findObjectByFriendlyName("mig")->position;
		//matView = glm::lookAt(camera.Position, migpos, camera.WorldUp);

		matView = camera.GetViewMatrix();

		glUniform3f(eyeLocation_location, camera.Position.x, camera.Position.y, camera.Position.z);

		//matView = glm::lookAt( g_CameraEye,	// Eye
		//	                    g_CameraAt,		// At
		//	                    glm::vec3( 0.0f, 1.0f, 0.0f ) );// Up

		glUniformMatrix4fv( matView_location, 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv( matProj_location, 1, GL_FALSE, glm::value_ptr(matProjection));
		// Do all this ONCE per frame
		LightManager->CopyLightValuesToShader();
			






		//std::sort(vec_sorted_drawObj.begin(), vec_sorted_drawObj.end(), transp);
		std::sort(vec_transObj.begin(), vec_transObj.end(), distToCam);
		
		cMeshObject* pSkyBox = findObjectByFriendlyName("SkyBoxObject");
		// Place skybox object at camera location
		pSkyBox->position = camera.Position;
		pSkyBox->bIsVisible = true;
		pSkyBox->bIsWireFrame = false;

		//		glDisable( GL_CULL_FACE );		// Force drawing the sphere
		//		                                // Could also invert the normals
				// Draw the BACK facing (because the normals of the sphere face OUT and we 
				//  are inside the centre of the sphere..
		//		glCullFace( GL_FRONT );

		// Bind the cube map texture to the cube map in the shader
		GLuint cityTextureUNIT_ID = 30;			// Texture unit go from 0 to 79
		glActiveTexture(cityTextureUNIT_ID + GL_TEXTURE0);	// GL_TEXTURE0 = 33984

		int cubeMapTextureID = ::g_pTheTextureManager->getTextureIDFromName("CityCubeMap");

		// Cube map is now bound to texture unit 30
		//		glBindTexture( GL_TEXTURE_2D, cubeMapTextureID );
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);

		//uniform samplerCube textureSkyBox;
		GLint skyBoxCubeMap_UniLoc = glGetUniformLocation(program, "textureSkyBox");
		glUniform1i(skyBoxCubeMap_UniLoc, cityTextureUNIT_ID);

		//uniform bool useSkyBoxTexture;
		GLint useSkyBoxTexture_UniLoc = glGetUniformLocation(program, "useSkyBoxTexture");
		glUniform1f(useSkyBoxTexture_UniLoc, (float)GL_TRUE);

		glm::mat4 matIdentity = glm::mat4(1.0f);
		DrawObject(pSkyBox, matIdentity, program);

		//		glEnable( GL_CULL_FACE );
		//		glCullFace( GL_BACK );

		pSkyBox->bIsVisible = false;
		glUniform1f(useSkyBoxTexture_UniLoc, (float)GL_FALSE);


		// Draw all the objects in the "scene"
		for ( unsigned int objIndex = 0; 
			  objIndex != (unsigned int)vec_non_transObj.size();
			  objIndex++ )
		{	
			cMeshObject* pCurrentMesh = vec_non_transObj[objIndex];
			
			glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;

			DrawObject(pCurrentMesh, matModel, program);

		}//for ( unsigned int objIndex = 0; 

		for (unsigned int objIndex = 0;
			objIndex != (unsigned int)vec_transObj.size();
			objIndex++)
		{
			cMeshObject* pCurrentMesh = vec_transObj[objIndex];

			glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;

			DrawObject(pCurrentMesh, matModel, program);

		}//for ( unsigned int objIndex = 0; 






		// High res timer (likely in ms or ns)
		currentTime = glfwGetTime();		
		deltaTime = currentTime - lastTime; 



		double MAX_DELTA_TIME = 0.1;	// 100 ms
		if (deltaTime > MAX_DELTA_TIME)
		{
			deltaTime = MAX_DELTA_TIME;
		}
		// update the "last time"
		lastTime = currentTime;

		for ( unsigned int objIndex = 0; 
			  objIndex != (unsigned int)vec_pObjectsToDraw.size(); 
			  objIndex++ )
		{	
			cMeshObject* pCurrentMesh = vec_pObjectsToDraw[objIndex];
			
			pCurrentMesh->Update( deltaTime );

		}//for ( unsigned int objIndex = 0; 

		//sceneCommandGroup.Update(deltaTime);
		


		// Call the debug renderer call
//#ifdef _DEBUG
		::g_pDebugRendererACTUAL->RenderDebugObjects( matView, matProjection, deltaTime );
//#endif 

		// update the "last time"
		

		// The physics update loop
		DoPhysicsUpdate( deltaTime, vec_pObjectsToDraw );

		cMeshObject* earth = findObjectByFriendlyName("earth");
		earth->adjMeshOrientationEulerAngles(glm::vec3(0.0f, 0.001f, 0.0f), false);
		findObjectByFriendlyName("clouds")->adjMeshOrientationEulerAngles(glm::vec3(0.0f, -0.0001f, 0.0f), false);


		
		sceneCommandGroup.Update(deltaTime);
		//::p_LuaScripts->UpdateCG(deltaTime);
		//::p_LuaScripts->Update(deltaTime);

		for (std::vector<sLight*>::iterator it = LightManager->vecLights.begin(); it != LightManager->vecLights.end(); ++it)
		{

			sLight* CurLight = *it;
			if (CurLight->AtenSphere == true)
			{


				cMeshObject* pDebugSphere = findObjectByFriendlyName("DebugSphere");
				pDebugSphere->bIsVisible = true;
				pDebugSphere->bDontLight = true;
				glm::vec4 oldDiffuse = pDebugSphere->materialDiffuse;
				glm::vec3 oldScale = pDebugSphere->nonUniformScale;
				pDebugSphere->setDiffuseColour(glm::vec3(255.0f / 255.0f, 105.0f / 255.0f, 180.0f / 255.0f));
				pDebugSphere->bUseVertexColour = false;
				pDebugSphere->position = glm::vec3(CurLight->position);
				glm::mat4 matBall(1.0f);


				pDebugSphere->materialDiffuse = oldDiffuse;
				pDebugSphere->setUniformScale(0.1f);			// Position
				DrawObject(pDebugSphere, matBall, program);

				const float ACCURACY_OF_DISTANCE = 0.0001f;
				const float INFINITE_DISTANCE = 10000.0f;

				float distance90Percent =
					pLightHelper->calcApproxDistFromAtten(0.90f, ACCURACY_OF_DISTANCE,
						INFINITE_DISTANCE,
						CurLight->atten.x,
						CurLight->atten.y,
						CurLight->atten.z);

				pDebugSphere->setUniformScale(distance90Percent);			// 90% brightness
				//pDebugSphere->objColour = glm::vec3(1.0f,1.0f,0.0f);
				pDebugSphere->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
				DrawObject(pDebugSphere, matBall, program);

				//			pDebugSphere->objColour = glm::vec3(0.0f,1.0f,0.0f);	// 50% brightness
				pDebugSphere->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
				float distance50Percent =
					pLightHelper->calcApproxDistFromAtten(0.50f, ACCURACY_OF_DISTANCE,
						INFINITE_DISTANCE,
						CurLight->atten.x,
						CurLight->atten.y,
						CurLight->atten.z);
				pDebugSphere->setUniformScale(distance50Percent);
				DrawObject(pDebugSphere, matBall, program);

				//			pDebugSphere->objColour = glm::vec3(1.0f,0.0f,0.0f);	// 25% brightness
				pDebugSphere->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
				float distance25Percent =
					pLightHelper->calcApproxDistFromAtten(0.25f, ACCURACY_OF_DISTANCE,
						INFINITE_DISTANCE,
						CurLight->atten.x,
						CurLight->atten.y,
						CurLight->atten.z);
				pDebugSphere->setUniformScale(distance25Percent);
				DrawObject(pDebugSphere, matBall, program);

				float distance1Percent =
					pLightHelper->calcApproxDistFromAtten(0.01f, ACCURACY_OF_DISTANCE,
						INFINITE_DISTANCE,
						CurLight->atten.x,
						CurLight->atten.y,
						CurLight->atten.z);
				//			pDebugSphere->objColour = glm::vec3(0.0f,0.0f,1.0f);	// 1% brightness
				pDebugSphere->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));
				pDebugSphere->setUniformScale(distance1Percent);
				DrawObject(pDebugSphere, matBall, program);

				//			pDebugSphere->objColour = oldColour;
				pDebugSphere->materialDiffuse = oldDiffuse;
				pDebugSphere->nonUniformScale = oldScale;
				pDebugSphere->bIsVisible = false;
			}
		}




		UpdateWindowTitle();

		glfwSwapBuffers(window);		// Shows what we drew

        glfwPollEvents();
		ProcessAsynKeys(window);




    }//while (!glfwWindowShouldClose(window))


	// Delete stuff
	delete pTheShaderManager;
	delete ::g_pTheVAOMeshManager;
	delete ::g_pTheTextureManager;

	// 
	delete ::g_pDebugRenderer;

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}





void UpdateWindowTitle(void)
{



	return;
}

cMeshObject* findObjectByFriendlyName(std::string theNameToFind)
{
	for ( unsigned int index = 0; index != vec_pObjectsToDraw.size(); index++ )
	{
		if ( vec_pObjectsToDraw[index]->friendlyName == theNameToFind )
		{
			return vec_pObjectsToDraw[index];
		}
	}

	// Didn't find it.
	return NULL;	// 0 or nullptr
}


cMeshObject* findObjectByUniqueID(unsigned int ID_to_find)
{
	for ( unsigned int index = 0; index != vec_pObjectsToDraw.size(); index++ )
	{
		if ( vec_pObjectsToDraw[index]->getUniqueID() == ID_to_find )
		{
			return vec_pObjectsToDraw[index];
		}
	}


	return NULL;	// 0 or nullptr
}


void LoadTerrainAABB(void)
{

	return;
}

#include <fstream>
void sendInput() {
	string Send_Message;
	std::string Recieve_Message;
	std::string ChatBuffer;
	vector<string> ConnRooms;
	// Buffer send
	Buffer* Send_Buffer = new Buffer(BUFFER_LENGTH);

	// Buffer recieve
	//Buffer* Recieve_Buffer = new Buffer(BUFFER_LENGTH);
	if (Conn.isAlive)
	{
		Recieve_Message = Conn.getMessages();
		//char* RoomName;
		//RoomName = new char[ARRAY_SIZE];
		cMeshObject* player = findObjectByFriendlyName("car");
		
		//Send_Message = std::to_string(msg_ids);
		Send_Message = "w";
		
		Conn.sendMessage(Send_Buffer, User, MSG_ID_INPUT, Conn.giveMsgID(), Send_Message);
		
		// Checking messege from the server
		std::cout << "print msg:" << std::endl;
		
		if (Recieve_Message == "CSP") {
			std::cout << " do CSP";
			player->position.z = 0.01;
		}else if (Recieve_Message != "") {

			ChatBuffer += Recieve_Message;

			cout << ChatBuffer;
			cout << '\n';
			double temp = ::atof(ChatBuffer.c_str());
			player->position.z = temp;

			//std::string s = ChatBuffer;
			//char delimiter = ':';

			//size_t pos = 0;
			//std::string token; 
			//std::vector<std::string> tokens;
			//float received[2];
			//int index = 0;
			//std::istringstream tokenStream(s);
			//while (std::getline(tokenStream, token, delimiter))
			//{
				//tokens.push_back(token);
			//}
			//while ((pos = s.find(delimiter)) != std::string::npos) {
			//	token = s.substr(0, pos);
			//	tokens.push_back(token);
			//	std::cout << token << std::endl;
			//	received[index] = std::stof(token);
			//	index++;
			//	//s.erase(0, pos + delimiter.length());

			//}
			//std::cout << s << std::endl;

			//double temp2 = ::atof(tokens[1].c_str());
			//std::cout << "id: " << tokens[0]<< std::endl;
			//std::cout << "z pos update: " << tokens[1] << std::endl;
			//float tempy = std::stof(tokens[1]);
			//player->position.z = tempy;
		}

	}
}