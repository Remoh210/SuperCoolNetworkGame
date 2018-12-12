// PlyFileLoader.cpp
#include "globalStuff.h"		// g_pArrayVert and g_pArrayTris

#include <string>
#include <iostream>			// cout, cin, etc. 
#include <fstream>			// fstream 

// Already in globalStuff.h
//#include "sPlyVertex.h"
//#include "sPlyTriangle.h"
//
// Each vertex looks like this in the ply file:
// -0.036872 0.127727 0.00440925 
//struct sPlyVertex {
////	sPlyVertex() : x(0.0f), y(0.0f), z(0.0f) {};
//	float x;
//	float y;
//	float z;
//};
//
//// 3 2603 2647 2602 
//struct sPlyTriangle	{	// "face"
//	int vertex_index_1;
//	int vertex_index_2;
//	int vertex_index_3;
//};

// Actual variables in memory
// NO extern here
sPlyVertex*		g_pArrayVert = 0;	// = new sPlyTriangle[numberOfTriangles];	// HEAP
unsigned int	g_numberOfVertices = 0;
sPlyTriangle*	g_pArrayTris = 0;	// NULL NULL  nullptr
unsigned int	g_numberOfTriangles = 0;

// Load the file and place it into a temporary 
//	set of arrays (in the CPU's RAM)
bool LoadPlyFileData(std::string fileName)
{
	// Open the file that you asked.
	std::ifstream theFile(fileName.c_str());

	// if ( theFile.is_open() == false )
	if (!theFile.is_open())			// More "c" or "C++" ish
	{
		std::cout << "Didn't open file" << std::endl;
		return false;
	}

	// file is open OK
	std::string nextData;
	while (theFile >> nextData)
	{
		if (nextData == "vertex")
		{
			break;		// exit while loop...
		}
	};
	// ...Jumping down to here

//	unsigned int numberOfVertices = 0;
	theFile >> g_numberOfVertices;

	std::cout << "vertices: " << g_numberOfVertices << std::endl;

	// seach for "face"
	while (theFile >> nextData)
	{
		if (nextData == "face")
		{
			break;		// exit while loop...
		}
	};
	// ...Jumping down to here

//	unsigned int numberOfTriangles = 0;
	theFile >> g_numberOfTriangles;
	std::cout << "triangles: " << g_numberOfTriangles << std::endl;

	while (theFile >> nextData)
	{
		if (nextData == "end_header")
		{
			break;		// exit while loop...
		}
	};
	// ...Jumping down to here

	//-0.036872 0.127727 0.00440925 
	//-0.0453607 0.128854 0.00114541 

	// Create an vertex array to store the data.
//	sPlyVertex tempVert; 
//	sPlyVertex tempArrayVert[1000];		// Static (stack)

//	sPlyVertex* pArrayVert = new sPlyVertex[numberOfVertices];	// HEAP
	g_pArrayVert = new sPlyVertex[g_numberOfVertices];	// HEAP

//	ZeroMemory(); win32
	// C call... (clears memory to all zeros)
	memset(g_pArrayVert, 0, sizeof(sPlyVertex) * g_numberOfVertices);

	// Read the vertex data into the array
	for (unsigned int index = 0; index != g_numberOfVertices; index++)
	{
		theFile >> g_pArrayVert[index].x;
		theFile >> g_pArrayVert[index].y;
		theFile >> g_pArrayVert[index].z;

		//		theFile >> g_pArrayVert[index].;
	}

	// Same with triangles

//	sPlyTriangle* pArrayTris = new sPlyTriangle[numberOfTriangles];	// HEAP
	g_pArrayTris = new sPlyTriangle[g_numberOfTriangles];	// HEAP

	memset(g_pArrayTris, 0, sizeof(sPlyTriangle) * g_numberOfTriangles);

	int TossThisAway = 0;
	for (unsigned int index = 0; index != g_numberOfTriangles; index++)
	{
		// 3 69 1322 70
		theFile >> TossThisAway;			// 3
		theFile >> g_pArrayTris[index].vertex_index_1;
		theFile >> g_pArrayTris[index].vertex_index_2;
		theFile >> g_pArrayTris[index].vertex_index_3;
	}

	std::cout << "Read from the file OK." << std::endl;

	return true;
}



//void saveLightInfo(std::string filename, std::vector<sLight*> lights)
//{
//	filename = "output/" + filename;
//	std::string line;
//	std::ofstream file(filename.c_str());
//	if (file.is_open())
//	{
//		for (std::vector<sLight*>::iterator it = lights.begin(); it != lights.end(); ++it) 
//		{
//			sLight* CurLight = *it;
//			file << "Position_xyz: " << CurLight->position.x << " " << CurLight->position.y << " " << CurLight->position.z << "\n";
//			file << "Attenuation_xyz: " << CurLight->atten.x << " " << CurLight->atten.y << " " << CurLight->atten.z << "\n";
//			file << "Diffuse_rgbw: " << CurLight->diffuse.x << " " << CurLight->diffuse.y << " " << CurLight->diffuse.z << " " << CurLight->diffuse.w << "\n";
//			file << "on_off: " << CurLight->param2.x << "\n";
//			//pTheOneLight->AtenSphere = false;
//			//LightManager->vecLights.push_back(pTheOneLight);
//			//LightManager->LoadUniformLocations(program);
//		}
//		std::cout << "Lights information saved to " << filename.c_str() << std::endl;
//		file.close();
//	}
//}


void saveLightInfo(std::string filename, std::vector<sLight*> lights)
{
	filename = "SceneInfo/" + filename;
	std::string line;
	std::ofstream file(filename.c_str());
	if (file.is_open())
	{
		for (std::vector<sLight*>::iterator it = lights.begin(); it != lights.end(); ++it)
		{
			sLight* CurLight = *it;
			file << "Name " << CurLight->lightName << "\n";
			file << "light_type " << CurLight->param1.x << "\n";
			file << "In_Out_Angles " << CurLight->param1.y << " " << CurLight->param1.z << "\n";
			file << "Position_xyz " << CurLight->position.x << " " << CurLight->position.y << " " << CurLight->position.z << "\n";
			file << "Direction " << CurLight->direction.x << " " << CurLight->direction.y << " " << CurLight->direction.z << "\n";
			file << "Attenuation_xyz " << CurLight->atten.x << " " << CurLight->atten.y << " " << CurLight->atten.z << "\n";
			file << "Diffuse_rgbw " << CurLight->diffuse.x << " " << CurLight->diffuse.y << " " << CurLight->diffuse.z << " " << CurLight->diffuse.w << "\n";
			file << "on_off " << CurLight->param2.x << "\n";


			//pTheOneLight->AtenSphere = false;
			//LightManager->vecLights.push_back(pTheOneLight);
			//LightManager->LoadUniformLocations(program);
		}
		std::cout << "Lights information saved to " << filename.c_str() << std::endl;
		file.close();
	}
}

//Save Models Info to the file
void saveModelInfo(std::string filename, std::vector<cMeshObject*> models)
{
	filename = "SceneInfo/" + filename;
	std::string line;
	std::ofstream file(filename.c_str());
	if (file.is_open())
	{
		for (std::vector<cMeshObject*>::iterator it = models.begin(); it != models.end(); ++it)
		{
			cMeshObject* CurModel = *it;
			if (CurModel->bIsDebug == false) {
				file << "Friendly_Name " << CurModel->friendlyName << "\n";
				file << "Mesh_Name " << CurModel->meshName << "\n";
				file << "Position " << CurModel->position.x << " " << CurModel->position.y << " " << CurModel->position.z << "\n";
				file << "DiffureRGB_Alpha  " << CurModel->materialDiffuse.x << " " << CurModel->materialDiffuse.y << " " << CurModel->materialDiffuse.z << " " << CurModel->materialDiffuse.w << "\n";
				file << "SpecularRGB_Power " << CurModel->materialSpecular.x << " " << CurModel->materialSpecular.y << " " << CurModel->materialSpecular.z << " " << CurModel->materialSpecular.w << "\n";
				file << "QuatXYZW_rotation " << CurModel->m_meshQOrientation.x << " " << CurModel->m_meshQOrientation.y << " " << CurModel->m_meshQOrientation.z << " " << CurModel->m_meshQOrientation.w << "\n";
				//			file << "Rotation " << CurModel->postRotation.x << " " << CurModel->postRotation.y << " " << CurModel->postRotation.z << "\n";
				file << "Scale  " << CurModel->nonUniformScale.x << " " << CurModel->nonUniformScale.y << " " << CurModel->nonUniformScale.z << "\n";
				file << "IsVisible  " << CurModel->bIsVisible << "\n";
				file << "IsUpdByPhys  " << CurModel->bIsUpdatedByPhysics << "\n";
				file << "IsWireFrame  " << CurModel->bIsWireFrame << "\n";
				if (CurModel->vecTextures.size() != 0) {
					file << "Textures: " << CurModel->vecTextures.size() << "\n";
					for (int i = 0; i < CurModel->vecTextures.size(); i++) {
						file << "Texture" + std::to_string(i) + "_name " << CurModel->vecTextures[i].name << "\n";
						file << "Texture" + std::to_string(i) + "_strength " << CurModel->vecTextures[i].strength << "\n";
					}
				}
				else { file << "Textures:  " << 0 << "\n"; }
				
			}

		//	file << "Colour  " << CurModel->objColour.x << " " << CurModel->objColour.y << " " << CurModel->objColour.z << "\n";
			//pTeapot->meshName = "Utah_Teapot_xyz_n.ply";
			//pTeapot->setUniformScale(0.4f);
		}
		std::cout << "Models information saved to " << filename.c_str() << std::endl;
		file.close();
	}
}







void saveCameraInfo(std::string filename)
{
	filename = "SceneInfo/" + filename;
	std::string line;
	std::ofstream file(filename.c_str());
	if (file.is_open())
	{

			file << "Camera_position " << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << "\n";
			file << "Camera_Speed " << camera.MovementSpeed << "\n";


	//	std::cout << "Models information saved to " << filename.c_str() << std::endl;
		file.close();
	}
}


void loadCameraInfo(std::string filename)
{
	std::ifstream file(("SceneInfo/" + filename).c_str());

	if (!file.is_open()) { return; }

		std::string unused;
		file >> unused >> camera.Position.x >> camera.Position.y >> camera.Position.z;
		file >> unused >> camera.MovementSpeed;

	file.close();
}
















//Load models Info from the file




//TXT OUTPUT LOOKS LIKE THIS
//Mesh_Name Utah_Teapot_xyz_n.ply
//Position - 150 60 0
//Rotation - 0.349066 0 0
//Scale 0.1 0.1 0.1
//Colour  0.2 0.8 0.3


void loadModels(std::string filename, std::vector<cMeshObject*> models)
{
	std::ifstream file(("SceneInfo/" + filename).c_str());

	if (!file.is_open()) { return; }

	std::map<std::string, cMeshObject*> meshObjects;
	for (unsigned int x = 0; x < models.size(); x++)
	{
		std::string friendlyName = models[x]->friendlyName;
		meshObjects[friendlyName] = models[x];
	}

	while (true)
	{
		int TexNumb = 0;
		std::string friendlyName, unused;
		file >> unused >> friendlyName;
		if (friendlyName == "") break;

		if (meshObjects.find(friendlyName) == meshObjects.end()) continue;

		std::cout << "Loading models " << friendlyName << std::endl;
		cMeshObject *CurModel = meshObjects.at(friendlyName);

		file >> unused >> CurModel->meshName;
		file >> unused >> CurModel->position.x >> CurModel->position.y >> CurModel->position.z;
		file >> unused >> CurModel->materialDiffuse.x >> CurModel->materialDiffuse.y >> CurModel->materialDiffuse.z >> CurModel->materialDiffuse.w;
		file >> unused >> CurModel->materialSpecular.x >> CurModel->materialSpecular.y >> CurModel->materialSpecular.z >> CurModel->materialSpecular.w;
		file >> unused >> CurModel->m_meshQOrientation.x >> CurModel->m_meshQOrientation.y >> CurModel->m_meshQOrientation.z >> CurModel->m_meshQOrientation.w;
		//		file >> unused >> CurModel->postRotation.x >> CurModel->postRotation.y >> CurModel->postRotation.z;
		file >> unused >> CurModel->nonUniformScale.x >> CurModel->nonUniformScale.y >> CurModel->nonUniformScale.z;
		file >> unused >> CurModel->bIsVisible;
		file >> unused >> CurModel->bIsUpdatedByPhysics;
		file >> unused >> CurModel->bIsWireFrame;
		file >> unused >> TexNumb;
		if (TexNumb > 0) {
			for (int i = 0; i < TexNumb; i++) {
				file >> unused >> CurModel->vecTextures[i].name;
				file >> unused >> CurModel->vecTextures[i].strength;
			}
		}
	}
	file.close();
}




void loadLights(std::string filename, std::vector<sLight*> lights)
{
	std::ifstream file(("SceneInfo/" + filename).c_str());

	if (!file.is_open()) { return; }

	std::map<std::string, sLight*> mapLights;
	for (unsigned int x = 0; x < lights.size(); x++)
	{
		std::string lightName = lights[x]->lightName;
		mapLights[lightName] = lights[x];
	}

	while (true)
	{
		std::string lightName, unused;
		file >> unused >> lightName;
		if (lightName == "") break;

		if (mapLights.find(lightName) == mapLights.end()) continue;

		std::cout << "Loading Lights " << lightName << std::endl;
		sLight *CurLight = mapLights.at(lightName);

		//file >> unused >> CurLight->lightName;
		file >> unused >> CurLight->param1.x;
		file >> unused >> CurLight->param1.y >> CurLight->param1.z;
		file >> unused >> CurLight->position.x >> CurLight->position.y >> CurLight->position.z;
		file >> unused >> CurLight->direction.x >> CurLight->direction.y >> CurLight->direction.z;
		file >> unused >> CurLight->atten.x >> CurLight->atten.y >> CurLight->atten.z;
		file >> unused >> CurLight->diffuse.x  >> CurLight->diffuse.y >> CurLight->diffuse.z >> CurLight->diffuse.w;
		file >> unused >> CurLight->param2.x;

	}
	file.close();
}





















void CreateModels(std::string filename, cVAOMeshManager* pTheVAOMeshManager, GLuint shaderProgramID)

{
	::g_pTheTextureManager->SetBasePath("assets/textures");
	std::ifstream file(("SceneInfo/" + filename).c_str());

	//if (!file.is_open()) { return; }

	//std::map<std::string, cMeshObject*> meshObjects;
	//for (unsigned int x = 0; x < models.size(); x++)
	//{
	//	std::string friendlyName = models[x]->friendlyName;
	//	meshObjects[friendlyName] = models[x];
	//}

	while (!file.eof())
	{

		

		
		int TexNumb = 0;
		std::string unused, friendlyName;
		file >> unused >> friendlyName;
	//	file >> unused >> friendlyName;
		if (friendlyName == "") break;

	//	if (meshObjects.find(friendlyName) == meshObjects.end()) continue;

	//	std::cout << "Loading models " << friendlyName << std::endl;
		cMeshObject *CurModel = new cMeshObject();
		sModelDrawInfo curModelInfo;
	//	ufoTexture.name = "metal.bmp";
//		ufoTexture.strength = 1.0f;
//		pTeapot->vecTextures.push_back(ufoTexture);
		
		CurModel->friendlyName = friendlyName;
		//
		//meshName.erase(meshName.end() - 4, meshName.end());
		//CurModel->friendlyName = meshName;

		

		file >> unused >> CurModel->meshName;
		file >> unused >> CurModel->position.x >> CurModel->position.y >> CurModel->position.z;
		file >> unused >> CurModel->materialDiffuse.r >> CurModel->materialDiffuse.g >> CurModel->materialDiffuse.b >> CurModel->materialDiffuse.a;
		file >> unused >> CurModel->materialSpecular.x >> CurModel->materialSpecular.y >> CurModel->materialSpecular.z >> CurModel->materialSpecular.w;
		file >> unused >> CurModel->m_meshQOrientation.x >> CurModel->m_meshQOrientation.y >> CurModel->m_meshQOrientation.z >> CurModel->m_meshQOrientation.w;
		//		file >> unused >> CurModel->postRotation.x >> CurModel->postRotation.y >> CurModel->postRotation.z;
		file >> unused >> CurModel->nonUniformScale.x >> CurModel->nonUniformScale.y >> CurModel->nonUniformScale.z;
		file >> unused >> CurModel->bIsVisible;
		file >> unused >> CurModel->bIsUpdatedByPhysics;
		file >> unused >> CurModel->bIsWireFrame;
		file >> unused >> TexNumb;
		if (TexNumb > 0) {
			for (int i = 0; i < TexNumb; i++) {
				sTextureInfo CurModelTex;
				file >> unused >> CurModelTex.name;
				file >> unused >> CurModelTex.strength;
				CurModel->vecTextures.push_back(CurModelTex);
				//Creating Texture even if there is alreade same textue NEED FIX
				::g_pTheTextureManager->Create2DTextureFromBMPFile(CurModelTex.name, true);
			}
		}

		curModelInfo.meshFileName = CurModel->meshName;
		vec_pObjectsToDraw.push_back(CurModel);
		pTheVAOMeshManager->LoadModelIntoVAO(curModelInfo, shaderProgramID);
		//		file >> unused >> CurModel->objColour.x >> CurModel->objColour.y >> CurModel->objColour.z;
	}
	file.close();
}














