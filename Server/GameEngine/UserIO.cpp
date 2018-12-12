#include "globalOpenGLStuff.h"
#include "globalStuff.h"
#include <string>
#include "Camera.h"
//#include <vector>
#include <iostream>
//include "TextureCTextureFromBMP.h"
float speed = 50.0f;
int index = 0;
// This has all the keyboard, mouse, and controller stuff

extern sLight* pTheOneLight;	//  = NULL;
extern cLightManager* LightManager;
int lightIndex = 0;
bool firstMouse = true;

void switchVis(cMeshObject* obj) {obj->bIsVisible = !obj->bIsVisible;}

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

std::string Answer;
void ManageScene(GLFWwindow* window);

bool bIsPicked = false;
cMeshObject* closedModel;
bool bMouseInWindow = false;

bool IsPicked = false;
void commandsInterface();

cMeshObject* CloseToObj(std::vector<cMeshObject*> models);

cMeshObject* cloesetObj;

void SwitchToWireFrame(std::vector<cMeshObject*> models);


void SwitchToSolid(std::vector<cMeshObject*> models);


void key_callback( GLFWwindow* window, 
						  int key, 
						  int scancode, 
						  int action, 
						  int mods)
{


    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		
        glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (glfwGetKey(window, GLFW_KEY_SLASH))
	{

		ManageScene(window);
		
	}



	//SAVE MODELS
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		//saveModelInfo("Models2.txt", vec_pObjectsToDraw);
		//saveLightInfo("lights.txt", LightManager->vecLights);
	}

	//LOAD MODELS
	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		SwitchToWireFrame(vec_pObjectsToDraw);
	}
	

	if (glfwGetKey(window, GLFW_KEY_K))
	{
		SwitchToSolid(vec_pObjectsToDraw);
	}


	if (glfwGetKey(window, GLFW_KEY_ENTER))
	{
		


	}





	if (glfwGetKey(window, GLFW_KEY_L))
	{
		loadModels("Models2.txt", vec_pObjectsToDraw);
		loadLights("lights.txt", LightManager->vecLights);
	}

	if (glfwGetKey(window, GLFW_KEY_R))
	{
		g_pTheTextureManager->filtType = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_T))
	{
		g_pTheTextureManager->filtType = 2;
	}

	//VISABILITY
	if (glfwGetKey(window, GLFW_KEY_SEMICOLON ))
	{
		switchVis(vec_pObjectsToDraw.at(index));
	}



	//Chose the model
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		if (index < (vec_pObjectsToDraw.size() - 1)) {
			
			index = index + 1;
		}
		else { index = 0; }
		std::cout << "Model " << vec_pObjectsToDraw.at(index)->meshName << " is Chosen" << std::endl;
	}


	//Chose the light
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		if (lightIndex < (LightManager->vecLights.size() - 1)) {

			lightIndex = lightIndex + 1;
		}
		else { lightIndex = 0; }
		std::cout << "Light " << LightManager->vecLights.at(lightIndex)->lightName << " is Chosen" << std::endl;
	}


	if (key == GLFW_KEY_9 && action == GLFW_PRESS)
	{


		//::p_LuaScripts->Update(deltaTime);

		//::p_LuaScripts->UpdateCG(deltaTime);

	}

	if (key == GLFW_KEY_8 && action == GLFW_PRESS)
	{


		//::p_LuaScripts->Update(deltaTime);

		//::p_LuaScripts->UpdateCG(deltaTime);
		
	}
	
	//TURN ON The Light

	if (key == GLFW_KEY_9 && action == GLFW_PRESS)
	{
		LightManager->vecLights.at(lightIndex)->param2.x = 1.0f;
	}
	//TURN OFF The Light
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		LightManager->vecLights.at(lightIndex)->param2.x = 0.0f;
	}







	//TURN ON AND OFF SPHERES THAT SHOW LIGHT ATTENUATION

	if (glfwGetKey(window, GLFW_KEY_O))
	{	
		LightManager->vecLights.at(lightIndex)->AtenSphere = true;
	}
	if (glfwGetKey(window, GLFW_KEY_P))
	{
		LightManager->vecLights.at(lightIndex)->AtenSphere = false;
	}


	
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{


		//LightManager->vecLights.at(lightIndex)->AtenSphere = false;
	}


	return;
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
		bMouseInWindow = true;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	if(bMouseInWindow)
	{
		camera.ProcessMouseMovement(xoffset, yoffset);
	}

}


bool IsShiftDown(GLFWwindow* window)
{
	if ( glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) )	{ return true; }
	if ( glfwGetKey( window, GLFW_KEY_RIGHT_SHIFT ) )	{ return true; }
	// both are up
	return false;
}

bool IsCtrlDown(GLFWwindow* window)
{
	if ( glfwGetKey( window, GLFW_KEY_LEFT_CONTROL ) )	{ return true; }
	if ( glfwGetKey( window, GLFW_KEY_RIGHT_CONTROL ) )	{ return true; }
	// both are up
	return false;
}

bool IsAltDown(GLFWwindow* window)
{
	if ( glfwGetKey( window, GLFW_KEY_LEFT_ALT ) )	{ return true; }
	if ( glfwGetKey( window, GLFW_KEY_RIGHT_ALT ) )	{ return true; }
	// both are up
	return false;
}

bool AreAllModifiersUp(GLFWwindow* window)
{
	if ( IsShiftDown(window) )	{ return false;	}
	if ( IsCtrlDown(window) )	{ return false;	} 
	if ( IsAltDown(window) )	{ return false; }
	// Yup, they are all UP
	//cMeshObject* player = findObjectByFriendlyName("Ufo2UVb");

	//player->bIsUpdatedByPhysics = true;
	if (glfwGetKey(window, GLFW_KEY_UP)) { 
		
		//player->accel += camera.Front * 1.01f;
		//player->velocity = camera.Front * 200.0f;
	}
	//else{ player->accel = -player->velocity * 1.0f; }
//	camera.Position = player->position + glm::vec3(0.0f, 0.0f, 0.5f);
	return true;
}

void ProcessAsynKeys(GLFWwindow* window)
{
	const float CAMERA_SPEED_SLOW = 5.0f;
	const float CAMERA_SPEED_FAST = 10.0f;

	// WASD + q = "up", e = down		y axis = up and down
	//									x axis = left and right
	//									z axis = forward and backward
	// 

	float cameraSpeed = CAMERA_SPEED_SLOW;
	if ( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS  )
	{
		cameraSpeed = CAMERA_SPEED_FAST;
	}
	
	// If no keys are down, move the camera
	if ( AreAllModifiersUp(window) )
	{
		// Note: The "== GLFW_PRESS" isn't really needed as it's actually "1" 
		// (so the if() treats the "1" as true...)

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			camera.ProcessKeyboard(UP, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			camera.ProcessKeyboard(DOWN, deltaTime);


	}//if(AreAllModifiersUp(window)

	//const float MIN_LIGHT_BRIGHTNESS = 0.001f;

	//LIGHT CONTROL*********************************************************************************************************
	if ( IsCtrlDown(window) )
	{
		
		
		
		if ( glfwGetKey( window, GLFW_KEY_W ) )	{	LightManager->vecLights.at(lightIndex)->position.z += cameraSpeed;	}
		if ( glfwGetKey( window, GLFW_KEY_S ) )	{	LightManager->vecLights.at(lightIndex)->position.z -= cameraSpeed;	}
		if ( glfwGetKey( window, GLFW_KEY_A ) )	{	LightManager->vecLights.at(lightIndex)->position.x -= cameraSpeed;	}
		if ( glfwGetKey( window, GLFW_KEY_D ) ) {	LightManager->vecLights.at(lightIndex)->position.x += cameraSpeed;	}
		if ( glfwGetKey( window, GLFW_KEY_Q ) )	{	LightManager->vecLights.at(lightIndex)->position.y += cameraSpeed;	}
		if ( glfwGetKey( window, GLFW_KEY_E ) )	{	LightManager->vecLights.at(lightIndex)->position.y -= cameraSpeed;	}


		//change colour of the light
		if ( glfwGetKey( window, GLFW_KEY_Z ) )	{   LightManager->vecLights.at(lightIndex)->diffuse = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); 	}//Red
		if ( glfwGetKey( window, GLFW_KEY_X ) )	{   LightManager->vecLights.at(lightIndex)->diffuse = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);	}//Green
		if ( glfwGetKey( window, GLFW_KEY_C ) )	{   LightManager->vecLights.at(lightIndex)->diffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);    }//Blue
		if ( glfwGetKey( window, GLFW_KEY_V ) )	{	LightManager->vecLights.at(lightIndex)->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);	}
		if ( glfwGetKey( window, GLFW_KEY_B ) )	{	LightManager->vecLights.at(lightIndex)->diffuse = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);	}


		//change lightype
		if (glfwGetKey(window, GLFW_KEY_1)) { LightManager->vecLights.at(lightIndex)->SetLightType(sLight::POINT_LIGHT); }//
		if (glfwGetKey(window, GLFW_KEY_2)) { LightManager->vecLights.at(lightIndex)->SetLightType(sLight::SPOT_LIGHT); }//

		//Change Light LookAt
		if (glfwGetKey(window, GLFW_KEY_R)) { LightManager->vecLights.at(lightIndex)->SetRelativeDirectionByLookAt(vec_pObjectsToDraw.at(index)->position); }



		//Change SpotCone
		if (glfwGetKey(window, GLFW_KEY_T))
		{
			LightManager->vecLights.at(lightIndex)->param1.y += 0.1f;

		}


		if (glfwGetKey(window, GLFW_KEY_Y))
		{
			LightManager->vecLights.at(lightIndex)->param1.y -= 0.1f;

		}


		if (glfwGetKey(window, GLFW_KEY_U))
		{
			LightManager->vecLights.at(lightIndex)->param1.z += 0.1f;
		}


		if (glfwGetKey(window, GLFW_KEY_I))
		{
			LightManager->vecLights.at(lightIndex)->param1.z -= 0.1f;
		}




		//change attenuation:

		//Linear
		if ( glfwGetKey( window, GLFW_KEY_LEFT)  )	{ LightManager->vecLights.at(lightIndex)->atten.y *= 1.05f; }
		if ( glfwGetKey( window, GLFW_KEY_RIGHT) )  { LightManager->vecLights.at(lightIndex)->atten.y *= 0.95f; }

		//Quadratic
		if ( glfwGetKey( window, GLFW_KEY_DOWN) )   { LightManager->vecLights.at(lightIndex)->atten.z *= 0.95f; }
		if ( glfwGetKey( window, GLFW_KEY_UP)   )	{ LightManager->vecLights.at(lightIndex)->atten.z *= 1.05f; }
		

	}



	//OBJECT CONTROL***********************************************************
	if ( IsAltDown(window) )
	{	//Object Postiton
		if ( glfwGetKey( window, GLFW_KEY_W	) )	{ vec_pObjectsToDraw.at(index)->position.z -= cameraSpeed; }
		if ( glfwGetKey( window, GLFW_KEY_S ) )	{ vec_pObjectsToDraw.at(index)->position.z += cameraSpeed; }
		if ( glfwGetKey( window, GLFW_KEY_A ) )	{ vec_pObjectsToDraw.at(index)->position.x -= cameraSpeed; }
		if ( glfwGetKey( window, GLFW_KEY_D ) ) { vec_pObjectsToDraw.at(index)->position.x += cameraSpeed; }
		if ( glfwGetKey( window, GLFW_KEY_Q ) )	{ vec_pObjectsToDraw.at(index)->position.y -= cameraSpeed; }
		if ( glfwGetKey( window, GLFW_KEY_E ) )	{ vec_pObjectsToDraw.at(index)->position.y += cameraSpeed; }

		////Object Rotation
		if (glfwGetKey(window, GLFW_KEY_RIGHT)) { vec_pObjectsToDraw.at(index)->adjMeshOrientationEulerAngles(0.0f, 0.1f, 0.0f, false); }
		if (glfwGetKey(window, GLFW_KEY_LEFT)) {vec_pObjectsToDraw.at(index)->adjMeshOrientationEulerAngles(0.0f, -0.1f, 0.0f, false);}
		//if ( glfwGetKey( window, GLFW_KEY_UP ) )	{ vec_pObjectsToDraw.at(index)->postRotation.x += 0.1f; }
		//if ( glfwGetKey( window, GLFW_KEY_DOWN ) )	{ vec_pObjectsToDraw.at(index)->postRotation.x -= 0.1f; }
		//if ( glfwGetKey( window, GLFW_KEY_X ) )		{ vec_pObjectsToDraw.at(index)->postRotation.z += 0.1f; }
		//if ( glfwGetKey( window, GLFW_KEY_C ) )		{ vec_pObjectsToDraw.at(index)->postRotation.z -= 0.1f; }

		if (glfwGetKey(window, GLFW_KEY_V)) { vec_pObjectsToDraw.at(index)->nonUniformScale += 0.2f; }
		if (glfwGetKey(window, GLFW_KEY_B)) { vec_pObjectsToDraw.at(index)->nonUniformScale -= 0.2f; }



		if (glfwGetKey(window, GLFW_KEY_UP))
		{
			if (vec_pObjectsToDraw.at(index)->vecTextures.size() > 1)
			{
				vec_pObjectsToDraw.at(index)->vecTextures.at(1).strength += 0.002f;
				vec_pObjectsToDraw.at(index)->vecTextures.at(0).strength -= 0.002f;
			}
			else { std::cout << vec_pObjectsToDraw.at(index)->friendlyName << " has only one texture" << std::endl; }
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN))
		{
			if (vec_pObjectsToDraw.at(index)->vecTextures.size() > 1)
			{
				vec_pObjectsToDraw.at(index)->vecTextures.at(1).strength -= 0.002f;
				vec_pObjectsToDraw.at(index)->vecTextures.at(0).strength += 0.002f;
			}
			else { std::cout << vec_pObjectsToDraw.at(index)->friendlyName << " has only one texture" << std::endl; }
		}

		

	}
	
	return;
}


void SwitchToWireFrame(std::vector<cMeshObject*> models)
{
	for (std::vector<cMeshObject*>::iterator it = models.begin(); it != models.end(); ++it)
	{
		cMeshObject* CurModel = *it;
		CurModel->bIsWireFrame = true;
	}
}

void SwitchToSolid(std::vector<cMeshObject*> models)
{
	for (std::vector<cMeshObject*>::iterator it = models.begin(); it != models.end(); ++it)
	{
		cMeshObject* CurModel = *it;
		CurModel->bIsWireFrame = false;

	}
}


void ManageScene(GLFWwindow* window)
{
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	bMouseInWindow = false;
	std::cout << "Save or Load? -s for save, -l for load, -c for cancel." << std::endl;
	std::cout << "-m to move selected object: " << vec_pObjectsToDraw[index]->friendlyName << std::endl;
	std::cout << "Enter Answer: " << std::endl;
	std::cout << "->";
	std::cin >> Answer;
	if (Answer == "-s" || Answer == "-S") {
		std::cout << "Which Scene to save? 1, 2?" << std::endl;
		std::cout << "->";
		std::cin >> Answer;
		if (Answer == "1") {
			saveModelInfo("Models.txt", vec_pObjectsToDraw);
			saveLightInfo("lights.txt", LightManager->vecLights);
			saveCameraInfo("camera.txt");
		}
		else if (Answer == "2") {
			saveModelInfo("Models2.txt", vec_pObjectsToDraw);
			saveLightInfo("lights2.txt", LightManager->vecLights);
			saveCameraInfo("camera2.txt");
		}
		else if (Answer == "3") {
			saveModelInfo("Models3.txt", vec_pObjectsToDraw);
			saveLightInfo("lights3.txt", LightManager->vecLights);
			saveCameraInfo("camera3.txt");
		}
		else{ std::cout << "cancelling.." << std::endl; }
	}
	else if (Answer == "-l") {
		std::cout << "Which Scene to load? 1, 2 or 3?" << std::endl;
		std::cout << "->";
		std::cin >> Answer;
		if (Answer == "1") {
			loadModels("Models.txt", vec_pObjectsToDraw);
			loadLights("lights.txt", LightManager->vecLights);
			loadCameraInfo("camera.txt");
		}
		else if (Answer == "2") {
			loadModels("Models2.txt", vec_pObjectsToDraw);
			loadLights("lights2.txt", LightManager->vecLights);
			loadCameraInfo("camera2.txt");
		}
		else if (Answer == "3") {
			loadModels("Models3.txt", vec_pObjectsToDraw);
			loadLights("lights3.txt", LightManager->vecLights);
			loadCameraInfo("camera3.txt");
		}
		else{ std::cout << "cancelling.." << std::endl; }
	}
	else if (Answer == "-m") {
		commandsInterface();
	}
	else {
		std::cout << Answer << std::endl;
		std::cout << "Cancelling.." << std::endl;
	}
	std::cout << "Click on Progrm Window to Continue" << std::endl;


	bMouseInWindow = true;
	firstMouse = true;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	
};



void commandsInterface()
{
	//cCommandGroup* pMoveToCG = new cCommandGroup();

	//cMoveToCommand*  pMoveTo = new cMoveToCommand();

	//std::vector<sNVPair> vecInitValues;

	//sNVPair ObjectToMove;	ObjectToMove.pMeshObj = vec_pObjectsToDraw[index];	

	std::cout << "Move object" << std::endl;
	std::cout << "1 - Move to object" << std::endl;
	std::cout << "2 - Move to coordinates" << std::endl;
	std::cout << "Enter Answer: " << std::endl;
	std::cout << "->";
	std::cin >> Answer;

	if(Answer == "1")
	{
		for (int i = 0; i < vec_pObjectsToDraw.size(); i++) { std::cout << vec_pObjectsToDraw[i]->friendlyName << std::endl; }
		std::cout << "Enter The name of the object: " << std::endl;
		std::cout << "->";
		std::cin >> Answer;
		//sNVPair Destination;	Destination.v3Value = findObjectByFriendlyName(Answer)->position;
		std::cout << "Enter Time in seconds: " << std::endl;
		std::cout << "->";
		std::cin >> Answer;
		//sNVPair Time;			Time.fValue = (float)atoi(Answer.c_str());
		//vecInitValues.push_back(ObjectToMove);	
		//vecInitValues.push_back(Destination);
		//vecInitValues.push_back(Time);
		

	}

	else if (Answer == "2")
	{
	//	glm::vec3 vec3MoveTo;
		std::cout << "Enter x: " << std::endl;
		std::cout << "->";
		std::cin >> Answer;
	//	vec3MoveTo.x = (float)atoi(Answer.c_str());
		std::cout << "Enter y: " << std::endl;
		std::cout << "->";
		std::cin >> Answer;
		//vec3MoveTo.y = (float)atoi(Answer.c_str());
		std::cout << "Enter z: " << std::endl;
		std::cout << "->";
		std::cin >> Answer;
		//vec3MoveTo.z = (float)atoi(Answer.c_str());
		
		std::cout << "Enter Time in seconds: " << std::endl;
		std::cout << "->";
		std::cin >> Answer;
		//sNVPair Destination;	Destination.v3Value = findObjectByFriendlyName(Answer)->position;
		//sNVPair Time;			Time.fValue = (float)atoi(Answer.c_str());
		//vecInitValues.push_back(ObjectToMove);	// idealCameraLocation
	//	vecInitValues.push_back(Destination);
	//	vecInitValues.push_back(Time);

	}

	//pMoveTo->Initialize(vecInitValues);

	//pMoveToCG->vecCommands.push_back(pMoveTo);

//	sceneCommandGroup.vecCommandGroups.push_back(pMoveToCG);
}