#include "globalOpenGLStuff.h"
#include "globalStuff.h"	
#include "cAABB.h"// for g_pRogerRabbit

#include "cVAOMeshManager.h"
#include "cMeshObject.h"

#include "DebugRenderer/cDebugRenderer.h"

#include <iostream>

// Loading models was moved into this function
void LoadModelTypes( cVAOMeshManager* pTheVAOMeshManager, GLuint shaderProgramID )
{

	//sModelDrawInfo tableInfo;
	//tableInfo.meshFileName = "table.ply";				
	//pTheVAOMeshManager->LoadModelIntoVAO(tableInfo, shaderProgramID);

	//sModelDrawInfo AxeInfo;
	//AxeInfo.meshFileName = "AxePainted.ply";					// "mig29_xyz.ply";
	//pTheVAOMeshManager->LoadModelIntoVAO(AxeInfo, shaderProgramID);
	//


	//sModelDrawInfo teapotInfo;
	//teapotInfo.meshFileName = "Utah_Teapot_xyz_n_GARBAGE_uv.ply";			// "Utah_Teapot_xyz.ply";
	//pTheVAOMeshManager->LoadModelIntoVAO(teapotInfo, shaderProgramID);

	//smodeldrawinfo ufoinfo;
	//ufoinfo.meshfilename = "ufo2uvb.ply";			// "utah_teapot_xyz.ply";
	//pthevaomeshmanager->loadmodelintovao(ufoinfo, shaderprogramid);

	//sModelDrawInfo terrainInfo;
	//terrainInfo.meshFileName = "MeshLab_Fractal_Terrain_xyz_n.ply";	// "MeshLab_Fractal_Terrain_xyz.ply";
	//// Will alow me to update the vertex data in the mesh
	//terrainInfo.bVertexBufferIsDynamic = true;
	//pTheVAOMeshManager->LoadModelIntoVAO(terrainInfo, shaderProgramID);
	//
	sModelDrawInfo sphereInfo;
	sphereInfo.meshFileName = "Sphere_320.ply";			// "Sphere_320_faces_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(sphereInfo, shaderProgramID);

	sModelDrawInfo sphereInvertedNormalsInfo;
	sphereInvertedNormalsInfo.meshFileName = "Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply";			// "Sphere_320_faces_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(sphereInvertedNormalsInfo, shaderProgramID);


	sModelDrawInfo mig;
	mig.meshFileName = "mig.ply";			// "Sphere_320_faces_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(mig, shaderProgramID);
	//sModelDrawInfo roomInfo;
	//roomInfo.meshFileName = "roomUV.ply";			// "Sphere_320_faces_xyz.ply";
	//pTheVAOMeshManager->LoadModelIntoVAO(roomInfo, shaderProgramID);



	//sModelDrawInfo skullInfo;
	//skullInfo.meshFileName = "skull.ply";	
	//skullInfo.bVertexBufferIsDynamic = true;
	//pTheVAOMeshManager->LoadModelIntoVAO(skullInfo, shaderProgramID);

	//sModelDrawInfo knightInfo;
	//knightInfo.meshFileName = "knight.ply";
	//knightInfo.bVertexBufferIsDynamic = true;
	//pTheVAOMeshManager->LoadModelIntoVAO(knightInfo, shaderProgramID);

	//sModelDrawInfo knight2Info;
	//knight2Info.meshFileName = "knight2.ply";
	//knight2Info.bVertexBufferIsDynamic = true;
	//pTheVAOMeshManager->LoadModelIntoVAO(knight2Info, shaderProgramID);

	//sModelDrawInfo fireInfo;
	//fireInfo.meshFileName = "fireplace.ply";
	//fireInfo.bVertexBufferIsDynamic = true;
	//pTheVAOMeshManager->LoadModelIntoVAO(fireInfo, shaderProgramID);

	//sModelDrawInfo lampInfo;
	//lampInfo.meshFileName = "lantern.ply";
	//lampInfo.bVertexBufferIsDynamic = true;
	//pTheVAOMeshManager->LoadModelIntoVAO(lampInfo, shaderProgramID);

	//sModelDrawInfo CatInfo;
	//CatInfo.meshFileName = "cat.ply";
	//CatInfo.bVertexBufferIsDynamic = true;
	//pTheVAOMeshManager->LoadModelIntoVAO(CatInfo, shaderProgramID);

	//sModelDrawInfo breadInfo;
	//breadInfo.meshFileName = "bread.ply";
	//breadInfo.bVertexBufferIsDynamic = true;
	//pTheVAOMeshManager->LoadModelIntoVAO(breadInfo, shaderProgramID);


	//for debug draw only ***********************
	sModelDrawInfo cube1x1x1;
	cube1x1x1.meshFileName = "cube_flat_shaded_xyz_n_uv.ply";			// "cube_flat_shaded_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(cube1x1x1, shaderProgramID);

	//sModelDrawInfo terrain;
	//terrain.meshFileName = "terrain.ply";			// "cube_flat_shaded_xyz.ply";
	//pTheVAOMeshManager->LoadModelIntoVAO(terrain, shaderProgramID);


	// At this point, mesh in in GPU
	std::cout << "Mesh was loaded OK" << std::endl;

	::g_pTheTextureManager->SetBasePath("assets/textures/cubemaps");
	std::string errorString;
	if (::g_pTheTextureManager->CreateCubeTextureFromBMPFiles("CityCubeMap",
		"city_lf.bmp", "city_rt.bmp",				// reverse these
		"city_dn.bmp", "city_up.bmp",				// Rotate the image "right 90 degrees")
		"city_ft.bmp", "city_bk.bmp", true, errorString))
	{
		std::cout << "Loaded the city cube map OK" << std::endl;
	}
	else
	{
		std::cout << "Error: city cube map DIDN't load. On no!" << std::endl;
	}
	//::g_pTheTextureManager->SetBasePath("assets/textures");

	//if (!::g_pTheTextureManager->Create2DTextureFromBMPFile("roomTex.bmp", true))
	//{
	//	std::cout << "Didn't load texture" << std::endl;
	//}
	//::g_pTheTextureManager->Create2DTextureFromBMPFile("grass.bmp", true);
	//::g_pTheTextureManager->Create2DTextureFromBMPFile("metal.bmp", true);
	//::g_pTheTextureManager->Create2DTextureFromBMPFile("wood.bmp", true);
	//::g_pTheTextureManager->Create2DTextureFromBMPFile("knightTex.bmp", true);
	//::g_pTheTextureManager->Create2DTextureFromBMPFile("knight2Tex.bmp", true);
	//::g_pTheTextureManager->Create2DTextureFromBMPFile("fireplace.bmp", true);
	//::g_pTheTextureManager->Create2DTextureFromBMPFile("lantern.bmp", true);
	//::g_pTheTextureManager->Create2DTextureFromBMPFile("cat.bmp", true);
	//::g_pTheTextureManager->Create2DTextureFromBMPFile("bread.bmp", true);
	//::g_pTheTextureManager->Create2DTextureFromBMPFile("AxeTex.bmp", true);

	

	return;
}

	
// Loads the models we are drawing into the vector
void LoadModelsIntoScene( std::vector<cMeshObject*> &vec_pObjectsToDraw )
{


	{	// This will be our "skybox" object.
		// (could also be a cube, or whatever)
		cMeshObject* pSkyBoxObject = new cMeshObject();
		pSkyBoxObject->setDiffuseColour(glm::vec3(1.0f, 105.0f / 255.0f, 180.0f / 255.0f));
		pSkyBoxObject->bUseVertexColour = false;
		pSkyBoxObject->friendlyName = "SkyBoxObject";
		float scale = 20000.0f;
		pSkyBoxObject->nonUniformScale = glm::vec3(scale, scale, scale);
		pSkyBoxObject->meshName = "Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply";			// "Sphere_320_faces_xyz.ply";
//		pSkyBoxObject->bIsWireFrame = true;
		pSkyBoxObject->bIsDebug = true;
		// Invisible until I need to draw it
		pSkyBoxObject->bIsVisible = false;

		vec_pObjectsToDraw.push_back(pSkyBoxObject);
	}


	{	// This sphere is the tiny little debug sphere
	cMeshObject* pPlayerSphere = new cMeshObject();
	//		pPlayerSphere->objColour = glm::vec3( 1.0f, 105.0f/255.0f, 180.0f/255.0f );
	pPlayerSphere->setDiffuseColour(glm::vec3(1.0f, 105.0f / 255.0f, 180.0f / 255.0f));
	pPlayerSphere->friendlyName = "DebugSphere";
	float scale = 1.0f;
	pPlayerSphere->nonUniformScale = glm::vec3(scale, scale, scale);
	pPlayerSphere->meshName = "Sphere_320.ply";
	pPlayerSphere->bIsWireFrame = true;

	pPlayerSphere->position = glm::vec3(-100.0f, 10.0f, 0.0f);
	//pPlayerSphere->velocity = glm::vec3(5.0f, 10.0f, 0.0f);		// starts at "up" at 10 mps
	//pPlayerSphere->accel = glm::vec3(0.0f, -5.0f, 0.0f);
	pPlayerSphere->bIsUpdatedByPhysics = true;
	pPlayerSphere->bIsDebug = true;

	pPlayerSphere->pDebugRenderer = ::g_pDebugRenderer;

	//pTerrain->nonUniformScale = glm::vec3(0.1f,0.1f,0.1f);
	vec_pObjectsToDraw.push_back(pPlayerSphere);
	}






	return;
}