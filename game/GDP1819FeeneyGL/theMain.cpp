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

//$(SourcePath)include;		// header (.h) files
//$(SourcePath)lib/x64;		// Library (.lib) files
//#include "include/GLFW/glfw3.h"
//#include "linmath.h"		***
#include <stdlib.h>
#include <stdio.h>		// printf();
#include <iostream>		// cout (console out)

#include <vector>		// "smart array" dynamic array

#include "cShaderManager.h"
#include "cMeshObject.h"
#include "cVAOMeshManager.h"

// Here' we include the actual class
// We will "pass around" the interface class pointer, though
#include "DebugRenderer/cDebugRenderer.h"

#include "cLightHelper.h"

#include "cAABB.h"

cDebugRenderer* g_pDebugRendererACTUAL = NULL;
iDebugRenderer* g_pDebugRenderer = NULL;



void UpdateWindowTitle(GLFWwindow* window);


void DoPhysicsUpdate( double deltaTime, 
					  std::vector< cMeshObject* > &vec_pObjectsToDraw );

std::vector< cMeshObject* > vec_pObjectsToDraw;

// To the right, up 4.0 units, along the x axis
//glm::vec3 g_lightPos = glm::vec3( 4.0f, 4.0f, 0.0f );
//float g_lightBrightness = 400000.0f;

unsigned int numberOfObjectsToDraw = 0;

//glm::vec3 g_CameraEye = glm::vec3( 0.0, -3.0, +100.0f );
//glm::vec3 g_CameraEye = glm::vec3( 32.0, 75.0, 136.0f );
//glm::vec3 g_CameraAt = glm::vec3( 0.0, 0.0, 0.0f );
cFlyCamera* g_pFlyCamera = NULL;

cJoystickManager* g_pJoysticks = NULL;

cShaderManager* pTheShaderManager = NULL;		// "Heap" variable
cVAOMeshManager* g_pTheVAOMeshManager = NULL;

cBasicTextureManager* g_pTheTextureManager = NULL;


// You could also search for the model by name, 
//	rather than using a global variable like this.
cMeshObject* g_pRogerRabbit = NULL;

// And example of the light
sLight* pTheOneLight = NULL;

// If true, then the "ambient brightness" spheres will be drawn
bool g_bDrawDebugLightSpheres = true;

// 
cAABBHierarchy* g_pTheTerrain = new cAABBHierarchy();


void DoPhysicsCheckpointNumberFour(double deltaTime);

// For now, I'm doing this here, but you might want to do this
//  in the object, in the "phsyics" thing, or wherever. 
//  Or leave it here!!
void LoadTerrainAABB(void);


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


int main(void)
{

//	unsigned long long AABB_ID = pAABB1->GetID();

//	pTerrainAABBThing->m_mapAABBs[AABB_ID] = pAABB1;



    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
	{
        exit(EXIT_FAILURE);
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(1024, 480, "Hello there", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

	// Mouse callbacks
	glfwSetCursorPosCallback( window, cursor_position_callback );
	glfwSetMouseButtonCallback( window, mouse_button_callback );
	glfwSetCursorEnterCallback( window, cursor_enter_callback );
	glfwSetScrollCallback( window, scroll_callback );
	glfwSetJoystickCallback(joystick_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

	::g_pJoysticks = new cJoystickManager();
	::g_pJoysticks->InitOrReset();

	// Joystick info
	std::cout << "Found: " << ::g_pJoysticks->getNumberOfJoysticksPresent() << " joysticks" << std::endl;
	if ( ::g_pJoysticks->getNumberOfJoysticksPresent() > 0 )
	{
		std::cout
			<< ">"
			<< ::g_pJoysticks->getJoystickName( ::g_pJoysticks->getFirstJoystickID() )
			<< "< is the 1st joystick found." << std::endl;
	}


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

	if ( pTheShaderManager->createProgramFromFile( "BasicUberShader", 
											       vertexShader, 
											       fragmentShader ) )
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

	// Camera creation
	::g_pFlyCamera = new cFlyCamera();


	GLuint program = pTheShaderManager->getIDFromFriendlyName("BasicUberShader");


	::g_pTheVAOMeshManager = new cVAOMeshManager();

	// Create the texture manager
	::g_pTheTextureManager = new cBasicTextureManager();



	// Loading the uniform variables here (rather than the inner draw loop)
	GLint objectColour_UniLoc = glGetUniformLocation( program, "objectColour" );
	//uniform vec3 lightPos;
	//uniform float lightAtten;
	//GLint lightPos_UniLoc = glGetUniformLocation( program, "lightPos" );
	//GLint lightBrightness_UniLoc = glGetUniformLocation( program, "lightBrightness" );
	
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

	if ( ! ::g_pDebugRendererACTUAL->initialize() )
	{
		std::cout << "Warning: couldn't init the debug renderer." << std::endl;
		std::cout << "\t" << ::g_pDebugRendererACTUAL->getLastError() << std::endl;
	}
	else
	{
		std::cout << "Debug renderer is OK" << std::endl;
	}


	



	// Loading models was moved into this function
	LoadModelTypes( ::g_pTheVAOMeshManager, program );
	LoadModelsIntoScene( ::vec_pObjectsToDraw );


	LoadTerrainAABB();

	// Get the current time to start with
	double lastTime = glfwGetTime();		



	// This would happen in the "light manager", where it would 
	//	do this for each light...
	pTheOneLight = new sLight();

	// Do this once...
	pTheOneLight->position_UniLoc = glGetUniformLocation( program, "theLights[0].position" );
	pTheOneLight->diffuse_UniLoc = glGetUniformLocation( program, "theLights[0].diffuse" );
	pTheOneLight->specular_UniLoc = glGetUniformLocation( program, "theLights[0].specular" );
	pTheOneLight->atten_UniLoc = glGetUniformLocation( program, "theLights[0].atten" );
	pTheOneLight->direction_UniLoc = glGetUniformLocation( program, "theLights[0].direction" );
	pTheOneLight->param1_UniLoc = glGetUniformLocation( program, "theLights[0].param1" );
	pTheOneLight->param2_UniLoc = glGetUniformLocation( program, "theLights[0].param2" );

//	pTheOneLight->position = glm::vec4( 25.0f, 25.0f, 0.0f, 1.0f );	
//	pTheOneLight->position = glm::vec4( 1.2f, 23.0f, 61.3f, 1.0f );	
	pTheOneLight->position = glm::vec4( 0.0f, 100.0f, 0.0f, 1.0f );	
	pTheOneLight->atten.x = 0.0f;				// 	float constAtten = 0.0f;
	pTheOneLight->atten.y = 0.00385720730f;		//	float linearAtten = 0.01f;
	pTheOneLight->atten.z = 0.00001f;		//	float quadAtten = 0.001f;
	pTheOneLight->diffuse = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );		// White light


	pTheOneLight->SetLightType( sLight::POINT_LIGHT );

//	pTheOneLight->SetLightType( sLight::SPOT_LIGHT );
//	pTheOneLight->SetSpotConeAngles( 45.0f, 45.0f );
////	pTheOneLight->SetSpotConeAngles( 15.0f, 45.0f );
//	// Direction is RELATIVE to the LIGHT (for spots)
//	// Straight down... 
//	pTheOneLight->SetRelativeDirection( glm::vec3(0.0f, -1.0f, 0.0f) );
//
////	pTheOneLight->SetLightType(sLight::DIRECTIONAL_LIGHT);
////	pTheOneLight->SetRelativeDirection( glm::vec3(-1.0f, 0.0f, 0.0f) );


	cLightHelper* pLightHelper = new cLightHelper();

// Used for the vertex displacement terrain exampls
//	float HACK_HEIGHT_MAP_RATIO_ADJUST = 1.0f;

	// ************************************************************************************************
	//    ___    _ _                                              _                           _     
	//   | __|__| | |_____ __ __  __ ___ _ __  _ __  __ _ _ _  __| |  _____ ____ _ _ __  _ __| |___ 
	//   | _/ _ \ | / _ \ V  V / / _/ _ \ '  \| '  \/ _` | ' \/ _` | / -_) \ / _` | '  \| '_ \ / -_)
	//   |_|\___/_|_\___/\_/\_/  \__\___/_|_|_|_|_|_\__,_|_||_\__,_| \___/_\_\__,_|_|_|_| .__/_\___|
	//                                                                                  |_|         
	// This is the basics of the "command based" pattern, suggested by FIRST robotics
	// Call update on this
	cCommandGroup sceneCommandGroup;

	// Wait...
	cCommandGroup* pWaitCG = new cCommandGroup();

	cWait* pWaitCommand = new cWait();
	std::vector<sNVPair> vecInitValuesWait;
	vecInitValuesWait.push_back( sNVPair(5.0f) );
	pWaitCommand->Initialize( vecInitValuesWait );
	// Add command to the "local" command group
	pWaitCG->vecCommands.push_back( pWaitCommand );
									 
	// Add this "local" command group to the "global" command group
	sceneCommandGroup.vecCommandGroups.push_back(pWaitCG);


	// Next is the "follow" AND "animation" running in parallel
	cCommandGroup* pFollowAnimatte_CG = new cCommandGroup();

	cFollowObjectCommand* pFollow = new cFollowObjectCommand();

	std::vector<sNVPair> vecInitValues;

	sNVPair idealCameraLocation;	idealCameraLocation.v3Value = ::g_pFlyCamera->eye;;	// idealCameraLocation
	sNVPair minDistance;			minDistance.fValue = 2.0;				// idealCameraLocation
	sNVPair maxSpeedDistance;		maxSpeedDistance.fValue = 25.0f;		// maxSpeedDistance
	sNVPair maxSpeed;				maxSpeed.fValue = 50.0f;				// maxSpeed
	sNVPair camLocation;			camLocation.v3Value = glm::vec3(-200.0f, 6.0f, 250.0f);

	vecInitValues.push_back(idealCameraLocation);	// idealCameraLocation
	vecInitValues.push_back(minDistance);				
	vecInitValues.push_back(maxSpeedDistance);	
	vecInitValues.push_back(maxSpeed);	
	vecInitValues.push_back(camLocation);

	pFollow->Initialize( vecInitValues );

	float HACK_HEIGHT_MAP_RATIO_ADJUST = 0.5f;


	pFollowAnimatte_CG->vecCommands.push_back( pFollow );


	// Add the animation command "in parallel" to the follow camera
	cAnimationCurveCommand* pAniCommand = new cAnimationCurveCommand();
	std::vector<sNVPair> vecAniCommandValues;
	// Normally, you'd place the data into this init vector...
	pAniCommand->Initialize(vecAniCommandValues);

	pFollowAnimatte_CG->vecCommands.push_back(pAniCommand);

	sceneCommandGroup.vecCommandGroups.push_back(pFollowAnimatte_CG);
	// ************************************************************************************************

	//
	cParticleEmitter gAPE;		//"A Particle Emitter"

	// This takes a while
	gAPE.Init( 5.0f, 5.0f, 
			   500, 
			   glm::vec3( 0.0f, 20.0f, 0.0f),
			   glm::vec3( -0.3  , 0.5 , -0.3  ),	// Min Vel
			   glm::vec3(  0.3  , 1.0 ,  0.3 ), // Max vel
			   glm::vec3( 0.0f, 0.5f,  0.0f ),	// Accel
			   4,				// Min # of particles TO BE CREATED AT ONCE
			   6 );			// Max # of particles TO BE CREATED AT ONCE
	bool bButtonPressed = false;

	// Draw the "scene" (run the program)
	while (!glfwWindowShouldClose(window))
    {

		// HACK: 
//		if ( ! bButtonPressed )
		{
			if ( glfwGetKey( window, GLFW_KEY_B ) )
			{
				// This is fast
				gAPE.Reset();
				bButtonPressed = true;
				std::cout << "Particle emitter fired!" << std::endl;
			}
		}//if ( ! bButtonPressed )


		// Switch to the shader we want
		::pTheShaderManager->useShaderProgram( "BasicUberShader" );

        float ratio;
        int width, height;
 		//glm::mat4x4 mvp;		
		// Now these are split into separate parts
		// (because we need to know whe
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
		matProjection = glm::perspective( 0.6f,			// FOV
			                                ratio,		// Aspect ratio
			                                0.1f,			// Near clipping plane
			                                10000.0f );	// Far clipping plane

		//matView = glm::lookAt( g_CameraEye,	// Eye
		//	                    g_CameraAt,		// At
		//	                    glm::vec3( 0.0f, 1.0f, 0.0f ) );// Up
		//matView = glm::lookAt( ::g_pFlyCamera->eye,	// Eye
		//	                   ::g_pFlyCamera->getAtInWorldSpace(),		// At
		//	                   glm::vec3( 0.0f, 1.0f, 0.0f ) );// Up

	// FLY AROUND CAMERA (mouse + keyboard control)
		matView = glm::lookAt( ::g_pFlyCamera->eye,	// Eye
			                   ::g_pFlyCamera->getAtInWorldSpace(),		// At
			                   ::g_pFlyCamera->getUpVector() );// Up

		// Getting the inforamtion from the pFollow command object
//		matView = glm::lookAt( pFollow->currentLocation,	// Eye
//			                   glm::vec3(0.0f, 0.0f, 0.0f),		// At
//			                   glm::vec3(0.0f, 1.0f, 0.0f) );// Up



//		glUniform3f( eyeLocation_location, ::g_CameraEye.x, ::g_CameraEye.y, ::g_CameraEye.z );
		glUniform3f( eyeLocation_location, ::g_pFlyCamera->eye.x, ::g_pFlyCamera->eye.y, ::g_pFlyCamera->eye.z );

		glUniformMatrix4fv( matView_location, 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv( matProj_location, 1, GL_FALSE, glm::value_ptr(matProjection));


		pTheOneLight->SetRelativeDirectionByLookAt( ::g_pRogerRabbit->position );

			
		// Do all this ONCE per frame
		glUniform4f( pTheOneLight->position_UniLoc, 
						pTheOneLight->position.x, pTheOneLight->position.y, pTheOneLight->position.z, 1.0f );
		glUniform4f( pTheOneLight->diffuse_UniLoc, 
						pTheOneLight->diffuse.x, pTheOneLight->diffuse.y, pTheOneLight->diffuse.z, 1.0f );
		glUniform4f( pTheOneLight->param2_UniLoc, 1.0f, 0.0f, 0.0f, 0.0f );	// Turns it "on")
		glUniform4f( pTheOneLight->atten_UniLoc, 
						pTheOneLight->atten.x, pTheOneLight->atten.y, pTheOneLight->atten.z, pTheOneLight->atten.w );

		// Now pass the things we need for spots and directional, too:
		glUniform4f( pTheOneLight->direction_UniLoc, 
						pTheOneLight->direction.x, 
						pTheOneLight->direction.y,
						pTheOneLight->direction.z,
						pTheOneLight->direction.w );
		glUniform4f( pTheOneLight->param1_UniLoc, 
						pTheOneLight->param1.x,		// lightType
						pTheOneLight->param1.y,		// inner angle
						pTheOneLight->param1.z,		// outer angle
						pTheOneLight->param1.w );	// TBD


		cMeshObject* pDebugSphere = findObjectByFriendlyName("DebugSphere");
		pDebugSphere->bIsVisible = true;
		pDebugSphere->bDontLight = true;
//			glm::vec3 oldColour = pDebugSphere->objColour;
		glm::vec4 oldDiffuse = pDebugSphere->materialDiffuse;
		glm::vec3 oldScale = pDebugSphere->nonUniformScale;
//			pDebugSphere->objColour = glm::vec3(255.0f/255.0f, 105.0f/255.0f, 180.0f/255.0f);
		pDebugSphere->setDiffuseColour( glm::vec3(255.0f/255.0f, 105.0f/255.0f, 180.0f/255.0f) );
		pDebugSphere->bUseVertexColour = false;
		pDebugSphere->position = glm::vec3(pTheOneLight->position);
		glm::mat4 matBall(1.0f);

		// For several reasons, the light isn't 'lit up',
		//	but to make it more visible, there's 3 'nested' 
		//	spheres. Look slightly to the right of the bunnies.
//			pDebugSphere->objColour = glm::vec3(1.0f,1.0f,1.0f);			
		pDebugSphere->materialDiffuse = oldDiffuse;
		pDebugSphere->setUniformScale(0.5f);			// Position
		DrawObject( pDebugSphere, matBall, program );

		if ( ::g_bDrawDebugLightSpheres )
		{


			const float ACCURACY_OF_DISTANCE = 0.0001f;
			const float INFINITE_DISTANCE = 10000.0f;

			float distance90Percent = 
				pLightHelper->calcApproxDistFromAtten( 0.90f, ACCURACY_OF_DISTANCE,
													INFINITE_DISTANCE,
												   pTheOneLight->atten.x, 
												   pTheOneLight->atten.y, 
												   pTheOneLight->atten.z );

			pDebugSphere->setUniformScale(distance90Percent);			// 90% brightness
			//pDebugSphere->objColour = glm::vec3(1.0f,1.0f,0.0f);
			pDebugSphere->setDiffuseColour( glm::vec3(1.0f,1.0f,0.0f) );
			DrawObject( pDebugSphere, matBall, program );

//			pDebugSphere->objColour = glm::vec3(0.0f,1.0f,0.0f);	// 50% brightness
			pDebugSphere->setDiffuseColour( glm::vec3(0.0f,1.0f,0.0f) );
			float distance50Percent = 
				pLightHelper->calcApproxDistFromAtten( 0.50f, ACCURACY_OF_DISTANCE,
													INFINITE_DISTANCE,
												   pTheOneLight->atten.x, 
												   pTheOneLight->atten.y, 
												   pTheOneLight->atten.z );
			pDebugSphere->setUniformScale(distance50Percent);
			DrawObject( pDebugSphere, matBall, program );

//			pDebugSphere->objColour = glm::vec3(1.0f,0.0f,0.0f);	// 25% brightness
			pDebugSphere->setDiffuseColour( glm::vec3(1.0f,0.0f,0.0f) );
			float distance25Percent = 
				pLightHelper->calcApproxDistFromAtten( 0.25f, ACCURACY_OF_DISTANCE,
													INFINITE_DISTANCE,
												   pTheOneLight->atten.x, 
												   pTheOneLight->atten.y, 
												   pTheOneLight->atten.z );
			pDebugSphere->setUniformScale(distance25Percent);
			DrawObject( pDebugSphere, matBall, program );

			float distance1Percent = 
				pLightHelper->calcApproxDistFromAtten( 0.01f, ACCURACY_OF_DISTANCE,
													INFINITE_DISTANCE,
												   pTheOneLight->atten.x, 
												   pTheOneLight->atten.y, 
												   pTheOneLight->atten.z );
//			pDebugSphere->objColour = glm::vec3(0.0f,0.0f,1.0f);	// 1% brightness
			pDebugSphere->setDiffuseColour( glm::vec3(0.0f,0.0f,1.0f) );
			pDebugSphere->setUniformScale(distance1Percent); 
			DrawObject( pDebugSphere, matBall, program );

//			pDebugSphere->objColour = oldColour;
			pDebugSphere->materialDiffuse = oldDiffuse;
			pDebugSphere->nonUniformScale = oldScale;
			pDebugSphere->bIsVisible = false;
		}//if(::g_bDrawDebugLightSpheres)


		{
			// ***************************************
			 // Draw the skybox first 
			cMeshObject* pSkyBox = findObjectByFriendlyName("SkyBoxObject");
//			cMeshObject* pSkyBox = findObjectByFriendlyName("SkyPirate");
			// Place skybox object at camera location
			pSkyBox->position = g_pFlyCamera->eye;
			pSkyBox->bIsVisible = true;
			pSkyBox->bIsWireFrame = false;

//			glm::vec3 oldPosition = pSkyBox->position;
//			glm::vec3 oldScale = pSkyBox->nonUniformScale;
//			pSkyBox->setUniformScale(100.0f);

//			glDisable( GL_CULL_FACE );		// Force drawing the sphere
	//		                                // Could also invert the normals
			// Draw the BACK facing (because the normals of the sphere face OUT and we 
			//  are inside the centre of the sphere..
//			glCullFace( GL_FRONT );

			// Bind the cube map texture to the cube map in the shader
			GLuint cityTextureUNIT_ID = 30;			// Texture unit go from 0 to 79
			glActiveTexture( cityTextureUNIT_ID + GL_TEXTURE0 );	// GL_TEXTURE0 = 33984

			int cubeMapTextureID = ::g_pTheTextureManager->getTextureIDFromName("CityCubeMap");

			// Cube map is now bound to texture unit 30
			//glBindTexture( GL_TEXTURE_2D, cubeMapTextureID );
			glBindTexture( GL_TEXTURE_CUBE_MAP, cubeMapTextureID );

			//uniform samplerCube textureSkyBox;
			GLint skyBoxCubeMap_UniLoc = glGetUniformLocation( program, "textureSkyBox" );
			glUniform1i( skyBoxCubeMap_UniLoc, cityTextureUNIT_ID );

			//uniform bool useSkyBoxTexture;
			GLint useSkyBoxTexture_UniLoc = glGetUniformLocation( program, "useSkyBoxTexture" );
			glUniform1f( useSkyBoxTexture_UniLoc, (float)GL_TRUE );

			glm::mat4 matIdentity = glm::mat4(1.0f);
			DrawObject( pSkyBox, matIdentity, program );

			//glEnable( GL_CULL_FACE );
			//glCullFace( GL_BACK );

			pSkyBox->bIsVisible = false;
			glUniform1f( useSkyBoxTexture_UniLoc, (float)GL_FALSE );

//			pSkyBox->position = oldPosition;
//			pSkyBox->nonUniformScale = oldScale;
			// ***************************************
		}


		std::vector< cMeshObject* > vec_pTransparentObject;


		// Draw all the objects in the "scene"
		for ( unsigned int objIndex = 0; 
			  objIndex != (unsigned int)vec_pObjectsToDraw.size(); 
			  objIndex++ )
		{	
			cMeshObject* pCurrentMesh = vec_pObjectsToDraw[objIndex];

			glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;

			DrawObject(pCurrentMesh, matModel, program);

		}//for ( unsigned int objIndex = 0; 


		// Draw a reflective bunny
		{
			GLint bAddReflect_UniLoc = glGetUniformLocation( program, "bAddReflect" );
//			glUniform1f( bAddReflect_UniLoc, (float)GL_TRUE );

			GLint bAddRefract_UniLoc = glGetUniformLocation( program, "bAddRefract" );
			glUniform1f( bAddRefract_UniLoc, (float)GL_TRUE );

			cMeshObject* pBunny = findObjectByFriendlyName("Roger");

			glm::vec3 oldPos = pBunny->position;
			glm::vec3 oldScale = pBunny->nonUniformScale;
			glm::quat oldOrientation = pBunny->getQOrientation();

			pBunny->position = glm::vec3(0.0f,25.0f,0.0f);
			pBunny->setUniformScale(100.0f);
			pBunny->setMeshOrientationEulerAngles( 0.0f, 0.0f, 0.0f, true );

			glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;

			DrawObject(pBunny, matModel, program);

			pBunny->position = oldPos;
			pBunny->nonUniformScale = oldScale;
			pBunny->setQOrientation(oldOrientation);

			glUniform1f( bAddReflect_UniLoc, (float)GL_FALSE );
			glUniform1f( bAddRefract_UniLoc, (float)GL_FALSE );
		}


		//{// Height map vertex displacement
		//	cMeshObject* pTerrain = findObjectByFriendlyName("The Terrain");

		//	glm::vec3 oldPos = pTerrain->position;
		//	pTerrain->position.y += 50.0f;
		//	pTerrain->bIsWireFrame = true;
		//	pTerrain->bUseVertexColour = true;
		//	pTerrain->setDiffuseColour( glm::vec3(1.0f, 1.0f, 1.0f ) );
		//	pTerrain->bDontLight = true;

		//	// Set all the things in our shader
		//	GLint texHeightMap_UniLoc = glGetUniformLocation( program, "texHeightMap");
		//	GLint bUseHeightMap_UniLoc = glGetUniformLocation( program, "bUseHeightMap");
		//	GLint heightMapRatio_UniLoc = glGetUniformLocation( program, "heightMapRatio");

		//	glUniform1f( bUseHeightMap_UniLoc, (float)GL_TRUE );

		//	//glUniform1f( heightMapRatio_UniLoc, 50.0f );
		//	glUniform1f( heightMapRatio_UniLoc, HACK_HEIGHT_MAP_RATIO_ADJUST );

		//	HACK_HEIGHT_MAP_RATIO_ADJUST += 0.1f;
		//	if ( HACK_HEIGHT_MAP_RATIO_ADJUST > 250.0f )
		//	{
		//		HACK_HEIGHT_MAP_RATIO_ADJUST = 0.0f;
		//	}

		//	// Texture binding to the texture unit
		//	GLuint heightTextureID = ::g_pTheTextureManager->getTextureIDFromName("rock_cave_stylized_height.bmp");

		//	GLuint textureUNIT_ID = 50;			// Texture unit go from 0 to 79
		//	glActiveTexture( textureUNIT_ID + GL_TEXTURE0 );	// GL_TEXTURE0 = 33984

		//	glBindTexture( GL_TEXTURE_2D, heightTextureID );

		//	glUniform1i( texHeightMap_UniLoc, textureUNIT_ID );
		//	//
	
		//	glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;
		//	DrawObject(pTerrain, matModel, program);

		//	glUniform1f( bUseHeightMap_UniLoc, (float)GL_FALSE );

		//	// Set everything back
		//	pTerrain->position = oldPos;
		//	pTerrain->bIsWireFrame = false;
		//	pTerrain->bUseVertexColour = false;
		//	pTerrain->bDontLight = false;
		//}// 



		// High res timer (likely in ms or ns)
		double currentTime = glfwGetTime();		
		double deltaTime = currentTime - lastTime; 


		double MAX_DELTA_TIME = 0.1;	// 100 ms
		if ( deltaTime > MAX_DELTA_TIME)
		{
			deltaTime = MAX_DELTA_TIME;
		}

		// Update the particle emitter
		cMeshObject* pParticleMesh = findObjectByFriendlyName("SmokeObject");
		glm::mat4 matParticleIndentity = glm::mat4(1.0f);
		glm::vec3 oldPosition = pParticleMesh->position;
		pParticleMesh->bIsVisible = true;

		// Set up the shader
		GLint bSmokeImposter_UniLoc = glGetUniformLocation( program, "bSmokeImposter" );
		glUniform1f( bSmokeImposter_UniLoc, (float)GL_TRUE );

		gAPE.Update(deltaTime);

		unsigned int numParticles = (unsigned int)gAPE.vecParticles.size();
		unsigned int count = 0;
		for ( unsigned int index = 0; index != numParticles; index++ )
		{
			if ( gAPE.vecParticles[index].lifeRemaining > 0.0f )
			{
				// Draw it
				pParticleMesh->position = gAPE.vecParticles[index].position;
				DrawObject( pParticleMesh, matParticleIndentity, program );
				count++;
			}
		}
		std::cout << "Drew " << count << " particles" << std::endl;
		pParticleMesh->bIsVisible = false;
		pParticleMesh->position = oldPosition;
		glUniform1f( bSmokeImposter_UniLoc, (float)GL_FALSE );




		for ( unsigned int objIndex = 0; 
			  objIndex != (unsigned int)vec_pObjectsToDraw.size(); 
			  objIndex++ )
		{	
			cMeshObject* pCurrentMesh = vec_pObjectsToDraw[objIndex];
			
			pCurrentMesh->Update( deltaTime );

		}//for ( unsigned int objIndex = 0; 


		// Update the entire "animation" command sequence
//		sceneCommandGroup.Update(deltaTime);

		// Move the "sky pirate" with that animation
		cMeshObject* pSkyPirate = findObjectByFriendlyName("SkyPirate");

		pSkyPirate->position = pAniCommand->currentLocation;

		pSkyPirate->setQOrientation( pAniCommand->qCurrentOrientation );





		// Rotate one of the rabbits over time
		float degreesThisFrame = (float)deltaTime * 10.0f;		// 10 degrees per second
		::g_pRogerRabbit->adjMeshOrientationEulerAngles( glm::vec3( degreesThisFrame, 0.0f, 0.0f ), true );




		// Call the debug renderer call
//#ifdef _DEBUG
		::g_pDebugRendererACTUAL->RenderDebugObjects( matView, matProjection, deltaTime );
//#endif 

	//	// **********************************************
	//{// START OF: AABB debug stuff
	//	//HACK: Draw Debug AABBs...

	//	// Get that from FindObjectByID()
	//	cMeshObject* pTheBunny = findObjectByFriendlyName("Roger");
	//	// Highlight the AABB that the rabbit is in (Or the CENTRE of the rabbit, anyway)

	//	float sideLength = 20.0f;
	//	unsigned long long ID_of_AABB_We_are_in = cAABB::generateID( pTheBunny->position, sideLength );

	//	// Is there a box here? 
	//	std::map< unsigned long long /*ID of the AABB*/, cAABB* >::iterator itAABB_Bunny			
	//		= ::g_pTheTerrain->m_mapAABBs.find(ID_of_AABB_We_are_in);

	//	// Is there an AABB there? 
	//	if ( itAABB_Bunny != ::g_pTheTerrain->m_mapAABBs.end() )
	//	{
	//		// Yes, then get the triangles and do narrow phase collision

	//		std::cout << "ID = " << ID_of_AABB_We_are_in 
	//			<< " with " << itAABB_Bunny->second->vecTriangles.size() <<" triangles" << std::endl;

	//		// *******************************************************************
	//		// Here you can pass this vector of triangles into your narrow phase (aka project #1)
	//		// and do whatever collision response you want
	//		// *******************************************************************
	//	}
	//	else
	//	{
	//		std::cout << "ID = " << ID_of_AABB_We_are_in << " NOT PRESENT near bunny" << std::endl;
	//	}


	//	std::map< unsigned long long /*ID of the AABB*/, cAABB* >::iterator itAABB 
	//		= ::g_pTheTerrain->m_mapAABBs.begin();
	//	for ( ; itAABB !=  ::g_pTheTerrain->m_mapAABBs.end(); itAABB++ )
	//	{
	//		// You could draw a mesh cube object at the location, 
	//		// but be careful that it's scalled and placed at the right location.
	//		// i.e. our cube is centred on the origin and is ++2++ units wide, 
	//		// because it's +1 unit from the centre (on all sides).

	//		// Since this is debug, and the "draw debug line thing" is working, 
	//		// this will just draw a bunch of lines... 

	//		cAABB* pCurrentAABB = itAABB->second;

	//		glm::vec3 cubeCorners[6];

	//		cubeCorners[0] = pCurrentAABB->getMinXYZ();
	//		cubeCorners[1] = pCurrentAABB->getMinXYZ();
	//		cubeCorners[2] = pCurrentAABB->getMinXYZ();
	//		cubeCorners[3] = pCurrentAABB->getMinXYZ();
	//		cubeCorners[4] = pCurrentAABB->getMinXYZ();
	//		cubeCorners[5] = pCurrentAABB->getMinXYZ();

	//		// Max XYZ
	//		cubeCorners[1].x += pCurrentAABB->getSideLength();
	//		cubeCorners[1].y += pCurrentAABB->getSideLength();
	//		cubeCorners[1].z += pCurrentAABB->getSideLength();

	//		cubeCorners[2].x += pCurrentAABB->getSideLength();

	//		cubeCorners[3].y += pCurrentAABB->getSideLength();

	//		cubeCorners[4].z += pCurrentAABB->getSideLength();

	//		// TODO: And the other corners... 
	//		cubeCorners[5].x += pCurrentAABB->getSideLength();
	//		cubeCorners[5].y += pCurrentAABB->getSideLength();

	//		// Draw line from minXYZ to maxXYZ
	//		::g_pDebugRenderer->addLine( cubeCorners[0], cubeCorners[1], 
	//									 glm::vec3(1,1,1), 0.0f );
	//	}
	//}// END OF: Scope for aabb debug stuff
	//// ********************************************************************************
		




		// update the "last time"
		lastTime = currentTime;

		// The physics update loop
		DoPhysicsUpdate( deltaTime, vec_pObjectsToDraw );


		// 
		DoPhysicsCheckpointNumberFour(deltaTime);


		//********************************************************
		// Can get that from the VAOMesh Manager...
		sModelDrawInfo theTerrainMesh;
		theTerrainMesh.meshFileName = "MeshLab_Fractal_Terrain_xyz_n_uv.ply";
//		theTerrainMesh.meshFileName = "singleTriangle_xyz.ply";
		::g_pTheVAOMeshManager->FindDrawInfoByModelName( theTerrainMesh );

		// Move the terrain a little bit...
//	for ( unsigned int index = 0; index != theTerrainMesh.numberOfVertices; index++ )
//	{
//		// There is an array of vertices like this:
//		//	sVert_xyz_rgb* pVerticesToVBO;	// = new sVert_xyz_rgb[ARRAYSIZE]
//
//		theTerrainMesh.pVerticesToVBO[index].y += 0.1f;
//		//theTerrainMesh.pVerticesToVBO[index].y *= sin(glfwGetTime()) * 1.0f;
//		theTerrainMesh.pVerticesToVBO[index].r = getRandBetween0and1<float>();
//		theTerrainMesh.pVerticesToVBO[index].g = getRandBetween0and1<float>();
//		theTerrainMesh.pVerticesToVBO[index].b = getRandBetween0and1<float>();
//	}

		// Change the terrain mesh (in the GPU)
		::g_pTheVAOMeshManager->UpdateModelVertexInformation( theTerrainMesh );



		// 

		// Get that from FindObjectByID()
		cMeshObject* pTheBunny = findObjectByFriendlyName("Roger");

		std::vector<sClosestPointData> vecClosestPoints;
		//CalculateClosestPointsOnMesh( theTerrainMesh, 
		//							  pTheBunny->position, 
		//							  vecClosestPoints );

		// Find the "closest triangle"
		// Go through each point and compare, finding the closest
		// (What if you have more than one that's the same distance?)
		// Assume the 1st one is the closet one.
		if ( vecClosestPoints.size() != 0 )
		{
			float minDistance = glm::distance( pTheBunny->position, vecClosestPoints[0].thePoint );
			unsigned int minTriangleIndex = vecClosestPoints[0].triangleIndex;

			for ( unsigned int triIndex = 0; triIndex != vecClosestPoints.size(); triIndex++ )
			{
			
				// glm::length() seems to work here, too 
				float curDist = glm::distance( pTheBunny->position, vecClosestPoints[triIndex].thePoint );

				if ( curDist < minDistance ) 
				{
					minDistance = curDist;
					minTriangleIndex = vecClosestPoints[triIndex].triangleIndex;
				}
			}
		}//if ( vecClosestPoints.size() != 0 )

//		std::cout << minTriangleIndex << std::endl;
//
//		// Mode the debug sphere to these points
//		// We can only draw one, so pick the 1st one..
//		cMeshObject* pDebugSphere = findObjectByFriendlyName("DebugSphere");
///
//		pDebugSphere->bIsVisible = true;
//		for ( unsigned int index = 0; index != vecClosestPoints.size(); index++ )
//		{
//			pDebugSphere->position = vecClosestPoints[index].thePoint;
//
//			// Draw it 
//			glm::mat4 matModel = glm::mat4(1.0f);	// Identity matrix
//
////			DrawObject( pDebugSphere, matModel, program );
//
//		}
//		pDebugSphere->bIsVisible = false;

		//********************************************************

		// At this point the scene is drawn...
		UpdateWindowTitle(window);

		glfwSwapBuffers(window);		// Shows what we drew

        glfwPollEvents();

		ProcessAsyncKeys(window);

		ProcessAsyncMouse(window);

		ProcessAsyncJoysticks(window, ::g_pJoysticks);



    }//while (!glfwWindowShouldClose(window))



	// Delete stuff
	delete pTheShaderManager;
	delete ::g_pTheVAOMeshManager;
	delete ::g_pTheTextureManager;

	// 
	delete ::g_pDebugRenderer;
	delete ::g_pFlyCamera;
	delete ::g_pJoysticks;

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}





void UpdateWindowTitle(GLFWwindow* window)
{
	// #include sstream 
	std::stringstream ssTitle;

	ssTitle			// std::cout 
		<< pTheOneLight->atten.x << ", " 
		<< pTheOneLight->atten.y << ", " 
		<< pTheOneLight->atten.z;

	glfwSetWindowTitle( window, ssTitle.str().c_str() );

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


// For physics checkpoint #4
// This is global because I need to update this with the keyboard
float g_ProjectileInitialVelocity = 10.0f;

void DoPhysicsCheckpointNumberFour(double deltaTime)
{
	// Draw a target right in front of the camera, like where it's pointing "At"...
	// (get it?? "AT" POinting AT!????)

	// THIS at is "just in front" of the camera.
	// It's exactly 1 unit away
	glm::vec3 cameraAt = ::g_pFlyCamera->getAtInWorldSpace();

	cMeshObject* pDebugBall = ::findObjectByFriendlyName("DebugSphere");

//	//Draw a red sphere at the "at" position of the camera ("right" in front of the camera)
//	pDebugBall->position = cameraAt;
//	pDebugBall->setDiffuseColour( glm::vec3(1.0f, 0.0f, 0.0f) );
//	pDebugBall->bIsVisible = true;
//	pDebugBall->setUniformScale( 0.1f );
//	
//	glm::mat4 matWorld = glm::mat4(1.0f);
//	DrawObject( pDebugBall, matWorld, ::pTheShaderManager->getIDFromFriendlyName("BasicUberShader") );
//	
//	pDebugBall->bIsVisible = false;


	// To draw a "trail" of spheres, we run the physics integration step
	// a number of times, based on the projectile velocity

	// Calculate the 3D "directed" velocity, assuming the projectile velocity is 
	//	"forward" from the camera...
	// 1. Get the camera "at" (direction of the camera)
	glm::vec3 projVelWorldSpace = ::g_pFlyCamera->getCameraDirection() * ::g_ProjectileInitialVelocity;
	glm::vec3 projPosition = ::g_pFlyCamera->eye;
	glm::vec3 projAccel = g_ACCEL_GRAVITY;


//	projVelWorldSpace = glm::vec3( 0.0f, 0.0f, 10.0f );


	// Run a number of steps of the simulation, drawing a sphere at X intervals of time

	float timeStep = 0.25f;
	float howLongWeGonnaRun = 10.0f;

	for ( float time = 0.0; time < howLongWeGonnaRun; time += timeStep )
	{
		// Stolen from the PhysicsUpdate()
		// Gravity is ???

		// Update velocity from acceleration...
		projVelWorldSpace.x = projVelWorldSpace.x + ( projAccel.x * timeStep );
		projVelWorldSpace.y = projVelWorldSpace.y + ( projAccel.y * timeStep );
		projVelWorldSpace.z = projVelWorldSpace.z + ( projAccel.z * timeStep );

		// Update position from velocity
		projPosition.x = projPosition.x + ( projVelWorldSpace.x * timeStep );
		projPosition.y = projPosition.y + ( projVelWorldSpace.y * timeStep );
		projPosition.z = projPosition.z + ( projVelWorldSpace.z * timeStep );

		// Draw a sphere at each of these locations...
		pDebugBall->position = projPosition;
		pDebugBall->setDiffuseColour( glm::vec3(0.8f, 0.8f, 0.8f) );
		pDebugBall->bIsVisible = true;
		pDebugBall->setUniformScale( 0.075f );

		glm::mat4 matWorld = glm::mat4(1.0f);
		DrawObject( pDebugBall, matWorld, ::pTheShaderManager->getIDFromFriendlyName("BasicUberShader") );

		pDebugBall->bIsVisible = false;

	}// for ( double time = 0.0;...


	return;
}


void LoadTerrainAABB(void)
{
	// *******
	// This REALLY should be inside the cAABBHierarchy, likely... 
	// *******


	// Read the graphics mesh object, and load the triangle info
	//	into the AABB thing.
	// Where is the mesh (do the triangles need to be transformed)??
	
	cMeshObject* pTerrain = findObjectByFriendlyName("The Terrain");

	sModelDrawInfo terrainMeshInfo;
	terrainMeshInfo.meshFileName = pTerrain->meshName;

	::g_pTheVAOMeshManager->FindDrawInfoByModelName( terrainMeshInfo );


	// How big is our AABBs? Side length?
	float sideLength = 20.0f;		// Play with this lenght
									// Smaller --> more AABBs, fewer triangles per AABB
									// Larger --> More triangles per AABB

	for ( unsigned int triIndex = 0; triIndex != terrainMeshInfo.numberOfTriangles; triIndex++ )
	{
		// for each triangle, for each vertex, determine which AABB the triangle should be in
		// (if your mesh has been transformed, then you need to transform the tirangles 
		//  BEFORE you do this... or just keep the terrain UNTRANSFORMED)

		sPlyTriangle currentTri = terrainMeshInfo.pTriangles[triIndex];

		sPlyVertex currentVerts[3];	
		currentVerts[0] = terrainMeshInfo.pVerticesFromFile[currentTri.vertex_index_1];
		currentVerts[1] = terrainMeshInfo.pVerticesFromFile[currentTri.vertex_index_2];
		currentVerts[2] = terrainMeshInfo.pVerticesFromFile[currentTri.vertex_index_3];

		// This is the structure we are eventually going to store in the AABB map...
		cAABB::sAABB_Triangle curAABBTri;
		curAABBTri.verts[0].x = currentVerts[0].x;
		curAABBTri.verts[0].y = currentVerts[0].y;
		curAABBTri.verts[0].z = currentVerts[0].z;
		curAABBTri.verts[1].x = currentVerts[1].x;
		curAABBTri.verts[1].y = currentVerts[1].y;
		curAABBTri.verts[1].z = currentVerts[1].z;
		curAABBTri.verts[2].x = currentVerts[2].x;
		curAABBTri.verts[2].y = currentVerts[2].y;
		curAABBTri.verts[2].z = currentVerts[2].z;

		// Is the triangle "too big", and if so, split it (take centre and make 3 more)
		// (Pro Tip: "too big" is the SMALLEST side is greater than HALF the AABB length)
		// Use THOSE triangles as the test (and recursively do this if needed),
		// +++BUT+++ store the ORIGINAL triangle info NOT the subdivided one
		// 
		// For the student to complete... 
		// 


		for ( unsigned int vertIndex = 0; vertIndex != 3; vertIndex++ )
		{
			// What AABB is "this" vertex in? 
			unsigned long long AABB_ID = 
				cAABB::generateID( curAABBTri.verts[0],
								   sideLength );

			// Do we have this AABB alredy? 
			std::map< unsigned long long/*ID AABB*/, cAABB* >::iterator itAABB
				= ::g_pTheTerrain->m_mapAABBs.find( AABB_ID );

			if( itAABB == ::g_pTheTerrain->m_mapAABBs.end() )
			{
				// We DON'T have an AABB, yet
				cAABB* pAABB = new cAABB();
				// Determine the AABB location for this point
				// (like the generateID() method...)
				glm::vec3 minXYZ = curAABBTri.verts[0];
				minXYZ.x = (floor(minXYZ.x / sideLength)) * sideLength;
				minXYZ.y = (floor(minXYZ.y / sideLength)) * sideLength;
				minXYZ.z = (floor(minXYZ.z / sideLength)) * sideLength;

				pAABB->setMinXYZ( minXYZ );
				pAABB->setSideLegth( sideLength );
				// Note: this is the SAME as the AABB_ID...
				unsigned long long the_AABB_ID = pAABB->getID();

				::g_pTheTerrain->m_mapAABBs[the_AABB_ID] = pAABB;

				// Then set the iterator to the AABB, by running find again
				itAABB = ::g_pTheTerrain->m_mapAABBs.find( the_AABB_ID );
			}//if( itAABB == ::g_pTheTerrain->m_mapAABBs.end() )

			// At this point, the itAABB ++IS++ pointing to an AABB
			// (either there WAS one already, or I just created on)

			itAABB->second->vecTriangles.push_back( curAABBTri );

		}//for ( unsigned int vertIndex = 0;

	}//for ( unsigned int triIndex



	// At runtime, need a "get the triangles" method...

	return;
}


