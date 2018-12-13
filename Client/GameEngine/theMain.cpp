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

GLuint program;
cDebugRenderer* g_pDebugRendererACTUAL = NULL;
iDebugRenderer* g_pDebugRenderer = NULL;
cLuaBrain* p_LuaScripts = NULL;
cCommandGroup sceneCommandGroup("SceneCG");
int cou;
std::vector<cAABB::sAABB_Triangle> vec_cur_AABB_tris;
void UpdateWindowTitle(void);
double currentTime = 0;
double deltaTime = 0;

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


//	GLint lightPos_UniLoc = glGetUniformLocation(program, "lightPos");
//	GLint lightBrightness_UniLoc = glGetUniformLocation(program, "lightBrightness");

	//	// uniform mat4 MVP;	THIS ONE IS NO LONGER USED	
	//uniform mat4 matModel;	// M
	//uniform mat4 matView;		// V
	//uniform mat4 matProj;		// P
	//GLint mvp_location = glGetUniformLocation(program, "MVP");
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
	//sLight* pTheOneLight = NULL;
	//sLight* pTheSecondLight = NULL;
	//sLight* pTheThirdLight = NULL;
	//sLight* pTheForthLight = NULL;

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

	//saveLightInfo("Default.txt")
	cLightHelper* pLightHelper = new cLightHelper();

	

	//Reload from the file
//	saveModelInfo("Models.txt", vec_pObjectsToDraw);
//	saveLightInfo("lights.txt", LightManager->vecLights);
	//loadModels("Models.txt", vec_pObjectsToDraw);
	loadLights("lights.txt", LightManager->vecLights);
	loadCameraInfo("camera.txt");
	//HACK; TODO save and load camera look at
	//camera.b_controlledByScript = true;
	//camera.SetViewMatrix(glm::lookAt(camera.Position, glm::vec3(285.0f, 245.0f, 825.0f), camera.WorldUp));
	




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
	sNVPair minDistance;				minDistance.fValue = 8;
	sNVPair maxSpeedDistance;			maxSpeedDistance.fValue = 30;
	sNVPair maxSpeed;					maxSpeed.fValue = 100;
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





		//REFLECTION

		//{
		//	GLint bAddReflect_UniLoc = glGetUniformLocation(program, "bAddReflect");
		//	//			glUniform1f( bAddReflect_UniLoc, (float)GL_TRUE );

		//	GLint bAddRefract_UniLoc = glGetUniformLocation(program, "bAddRefract");
		//	glUniform1f(bAddRefract_UniLoc, (float)GL_TRUE);

		//	cMeshObject* pBunny = findObjectByFriendlyName("Ufo2UVb");

		//	glm::vec3 oldPos = pBunny->position;
		//	glm::vec3 oldScale = pBunny->nonUniformScale;
		//	glm::quat oldOrientation = pBunny->getQOrientation();

		//	pBunny->position = glm::vec3(0.0f, 25.0f, 0.0f);
		//	pBunny->setUniformScale(100.0f);
		//	pBunny->setMeshOrientationEulerAngles(0.0f, 0.0f, 0.0f, true);

		//	glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;

		//	DrawObject(pBunny, matModel, program);

		//	pBunny->position = oldPos;
		//	pBunny->nonUniformScale = oldScale;
		//	pBunny->setQOrientation(oldOrientation);

		//	glUniform1f(bAddReflect_UniLoc, (float)GL_FALSE);
		//	glUniform1f(bAddRefract_UniLoc, (float)GL_FALSE);
		//}





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
		// Is this it? 500K - 1M
		// CPU limited Memory delay = 0
		// CPU over powered (x100 x1000) Memory is REAAAAALLY SLOW
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

	// Didn't find it.
	return NULL;	// 0 or nullptr
}


void LoadTerrainAABB(void)
{
	// *******
	// This REALLY should be inside the cAABBHierarchy, likely... 
	// *******


	// Read the graphics mesh object, and load the triangle info
	//	into the AABB thing.
	// Where is the mesh (do the triangles need to be transformed)??

//	cMeshObject* pTerrain = findObjectByFriendlyName("terrain");

//	sModelDrawInfo terrainMeshInfo;
//	terrainMeshInfo.meshFileName = pTerrain->meshName;

//	::g_pTheVAOMeshManager->FindDrawInfoByModelName(terrainMeshInfo);


	//// How big is our AABBs? Side length?
	//float sideLength = 50.0f;		// Play with this lenght
	//								// Smaller --> more AABBs, fewer triangles per AABB
	//								// Larger --> More triangles per AABB	

	//for (unsigned int triIndex = 0; triIndex != terrainMeshInfo.numberOfTriangles; triIndex++)
	//{
	//	// for each triangle, for each vertex, determine which AABB the triangle should be in
	//	// (if your mesh has been transformed, then you need to transform the tirangles 
	//	//  BEFORE you do this... or just keep the terrain UNTRANSFORMED)

	//	sPlyTriangle currentTri = terrainMeshInfo.pTriangles[triIndex];
	//	

	//	sPlyVertex currentVerts[3];
	//	currentVerts[0] = terrainMeshInfo.pVerticesFromFile[currentTri.vertex_index_1];
	//	currentVerts[1] = terrainMeshInfo.pVerticesFromFile[currentTri.vertex_index_2];
	//	currentVerts[2] = terrainMeshInfo.pVerticesFromFile[currentTri.vertex_index_3];

	//	// This is the structure we are eventually going to store in the AABB map...
	//	cAABB::sAABB_Triangle curAABBTri;
	//	curAABBTri.verts[0].x = currentVerts[0].x;
	//	curAABBTri.verts[0].y = currentVerts[0].y;
	//	curAABBTri.verts[0].z = currentVerts[0].z;
	//	curAABBTri.verts[1].x = currentVerts[1].x;
	//	curAABBTri.verts[1].y = currentVerts[1].y;
	//	curAABBTri.verts[1].z = currentVerts[1].z;
	//	curAABBTri.verts[2].x = currentVerts[2].x;
	//	curAABBTri.verts[2].y = currentVerts[2].y;
	//	curAABBTri.verts[2].z = currentVerts[2].z;

	//	// Is the triangle "too big", and if so, split it (take centre and make 3 more)
	//	// (Pro Tip: "too big" is the SMALLEST side is greater than HALF the AABB length)
	//	// Use THOSE triangles as the test (and recursively do this if needed),
	//	// +++BUT+++ store the ORIGINAL triangle info NOT the subdivided one
	//	// 
	//	// For the student to complete... 
	//	// 


	//	for (unsigned int vertIndex = 0; vertIndex != 3; vertIndex++)
	//	{
	//		// What AABB is "this" vertex in? 
	//		unsigned long long AABB_ID =
	//			cAABB::generateID(curAABBTri.verts[0],
	//				sideLength);

	//		// Do we have this AABB alredy? 
	//		std::map< unsigned long long/*ID AABB*/, cAABB* >::iterator itAABB
	//			= ::g_pTheTerrain->m_mapAABBs.find(AABB_ID);

	//		if (itAABB == ::g_pTheTerrain->m_mapAABBs.end())
	//		{
	//			// We DON'T have an AABB, yet



	//			std::cout << cou++ << std::endl;



	//			cAABB* pAABB = new cAABB();
	//			// Determine the AABB location for this point
	//			// (like the generateID() method...)
	//			glm::vec3 minXYZ = curAABBTri.verts[0];
	//			minXYZ.x = (floor(minXYZ.x / sideLength)) * sideLength;
	//			minXYZ.y = (floor(minXYZ.y / sideLength)) * sideLength;
	//			minXYZ.z = (floor(minXYZ.z / sideLength)) * sideLength;

	//			//pAABB->setMinXYZ(minXYZ);
	//			//pAABB->setSideLegth(sideLength);

	//			pAABB->setCenter(minXYZ + sideLength / 2);
	//			pAABB->setHalfLegth(sideLength/2);

	//			// Note: this is the SAME as the AABB_ID...
	//			unsigned long long the_AABB_ID = pAABB->getID();

	//			::g_pTheTerrain->m_mapAABBs[the_AABB_ID] = pAABB;

	//			// Then set the iterator to the AABB, by running find again
	//			itAABB = ::g_pTheTerrain->m_mapAABBs.find(the_AABB_ID);



	//			//cMeshObject* pCubeForBallsToBounceIn = new cMeshObject();
	//			//
	//			//pCubeForBallsToBounceIn->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
	//			//pCubeForBallsToBounceIn->bDontLight = true;
	//			//pCubeForBallsToBounceIn->position = pAABB->getCentre();
	//			//pCubeForBallsToBounceIn->friendlyName = "CubeBallsBounceIn";
	//			//pCubeForBallsToBounceIn->meshName = "cube_flat_shaded_xyz_n_uv.ply";		// "cube_flat_shaded_xyz.ply";
	//			//pCubeForBallsToBounceIn->setUniformScale(sideLength);
	//			//pCubeForBallsToBounceIn->bIsWireFrame = true;
	//			// Cube is 2x2x2, so with a scale of 100x means it's
	//			//	200x200x200, centred around the origin (0,0,0)
	//			// The GROUND_PLANE_Y = -3.0f, so place it +97.0 lines up the 'bottom'
	//			//pCubeForBallsToBounceIn->position = glm::vec3(0.0f, 97.0f, 0.0f);
	//			//pCubeForBallsToBounceIn->bIsWireFrame = true;

	//		//	pCubeForBallsToBounceIn->pDebugRenderer = ::g_pDebugRenderer;

	//			//pTerrain->nonUniformScale = glm::vec3(0.1f,0.1f,0.1f);
	//			//vec_pObjectsToDraw.push_back(pCubeForBallsToBounceIn);


	//		}//if( itAABB == ::g_pTheTerrain->m_mapAABBs.end() )

	//		// At this point, the itAABB ++IS++ pointing to an AABB
	//		// (either there WAS one already, or I just created on)

	//		itAABB->second->vecTriangles.push_back(curAABBTri);

	//	}//for ( unsigned int vertIndex = 0;

	//}//for ( unsigned int triIndex



	// At runtime, need a "get the triangles" method...

	return;
}




