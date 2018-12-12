#include "globalStuff.h"		
#include "globalOpenGLStuff.h"		// For the glad and glfw stuff

#include "cVAOMeshManager.h"

#include <fstream>
#include <string>
#include <iostream> 
#include <sstream>

cVAOMeshManager::cVAOMeshManager()		// constructor
{
	return;
}

cVAOMeshManager::~cVAOMeshManager()		// destructor
{
	// Will call ShutDown();
	return;
}

	// Note: the shader program ID is needed to tie 
	// the buffer to the vertex layout of the shader
bool cVAOMeshManager::LoadModelIntoVAO( 
		sModelDrawInfo &drawInfo,	
		unsigned int shaderProgramID )
{
	// See if this model has already been loaded
	std::map< std::string /*model name*/, sModelDrawInfo >::iterator itModel = this->m_mapModel_to_VAOID.find( drawInfo.meshFileName );
	
	if ( itModel != this->m_mapModel_to_VAOID.end() )
	{
		this->m_AppendTextToLastError( "Model ", false );
		this->m_AppendTextToLastError( drawInfo.meshFileName, false );
		this->m_AppendTextToLastError( " was already loaded.");
		return false;
	}



	// Load the model from the file
	if ( ! this->m_LoadModelFromFile( drawInfo ) )
	{
		// Add some error code
		return false;
	}

	// Load the model data into the GPU
	//LoadMeshIntoGPUBuffer
	if ( ! this->m_LoadDrawInfo_Into_VAO( drawInfo, shaderProgramID ) )
	{
		// Add some error text
		return false;
	}
								  
	// Store this mesh into the map
	this->m_mapModel_to_VAOID[drawInfo.meshFileName] = drawInfo;

	return true;
}

// Looks up draw info by name (meshFileName)			
bool cVAOMeshManager::FindDrawInfoByModelName( sModelDrawInfo &drawInfo )
{
	std::map< std::string /*model name*/, sModelDrawInfo >::iterator itModel = this->m_mapModel_to_VAOID.find( drawInfo.meshFileName );

	if ( itModel == this->m_mapModel_to_VAOID.end() )
	{
		// Didn't find model
		return false;
	}

	// Copy the draw info back to the caller.
	drawInfo = itModel->second;

	return true;
}


bool cVAOMeshManager::m_LoadModelFromFile( sModelDrawInfo &drawInfo )
{
	// Open the file that you asked.
	std::ifstream theFile( drawInfo.meshFileName.c_str() );	

	// if ( theFile.is_open() == false )
	if ( ! theFile.is_open() )			// More "c" or "C++" ish
	{
		//std::cout << "Didn't open file" << std::endl;
		this->m_AppendTextToLastError( "Didn't load the ", false );
		this->m_AppendTextToLastError( drawInfo.meshFileName, false );
		this->m_AppendTextToLastError( " file." );
		return false;
	}//if (!theFile.is_open() 
	
	// file is open OK
	std::string nextData;
	while ( theFile >> nextData )
	{
		if ( nextData == "vertex" ) 
		{
			break;		// exit while loop...
		}
	};
	// ...Jumping down to here

//	unsigned int numberOfVertices = 0;
//	theFile >> g_numberOfVertices;
	theFile >> drawInfo.numberOfVertices;

//	std::cout << "vertices: " << g_numberOfVertices << std::endl;
	std::cout << "vertices: " << drawInfo.numberOfVertices << std::endl;

	// seach for "face"
	while ( theFile >> nextData )
	{
		if ( nextData == "face" ) 
		{
			break;		// exit while loop...
		}
	};
	// ...Jumping down to here

//	unsigned int numberOfTriangles = 0;
//	theFile >> g_numberOfTriangles;
	theFile >> drawInfo.numberOfTriangles;

//	std::cout << "triangles: " << g_numberOfTriangles << std::endl;
	std::cout << "triangles: " << drawInfo.numberOfTriangles << std::endl;

	while ( theFile >> nextData )
	{
		if ( nextData == "end_header" ) 
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
//	g_pArrayVert = new sPlyVertex[g_numberOfVertices];	// HEAP
	drawInfo.pVerticesFromFile = new sPlyVertex[drawInfo.numberOfVertices];

//	ZeroMemory(); win32
	// C call... (clears memory to all zeros)
//	memset( g_pArrayVert, 0, sizeof( sPlyVertex ) * g_numberOfVertices );
	memset( drawInfo.pVerticesFromFile, 0, sizeof( sPlyVertex ) * drawInfo.numberOfVertices );

	// Read the vertex data into the array
	for ( unsigned int index = 0; index != drawInfo.numberOfVertices; index++ )
	{
		theFile >> drawInfo.pVerticesFromFile[index].x;
		theFile >> drawInfo.pVerticesFromFile[index].y;
		theFile >> drawInfo.pVerticesFromFile[index].z;

		theFile >> drawInfo.pVerticesFromFile[index].nx;
		theFile >> drawInfo.pVerticesFromFile[index].ny;
		theFile >> drawInfo.pVerticesFromFile[index].nz;

		//  Also load the UV values
		theFile >> drawInfo.pVerticesFromFile[index].u;
		theFile >> drawInfo.pVerticesFromFile[index].v;

//		theFile >> g_pArrayVert[index].;
	}//for ( unsigned int index...

	// Same with triangles

//	sPlyTriangle* pArrayTris = new sPlyTriangle[numberOfTriangles];	// HEAP
//	g_pArrayTris = new sPlyTriangle[g_numberOfTriangles];	// HEAP
	drawInfo.pTriangles = new sPlyTriangle[drawInfo.numberOfTriangles];

//	memset( g_pArrayTris, 0, sizeof( sPlyTriangle ) * g_numberOfTriangles );
	memset( drawInfo.pTriangles, 0, sizeof( sPlyTriangle ) * drawInfo.numberOfTriangles );

	int TossThisAway = 0;
	for ( unsigned int index = 0; index !=  drawInfo.numberOfTriangles; index++ )
	{
		// 3 69 1322 70
		theFile >> TossThisAway;			// 3
		theFile >> drawInfo.pTriangles[index].vertex_index_1;
		theFile >> drawInfo.pTriangles[index].vertex_index_2;
		theFile >> drawInfo.pTriangles[index].vertex_index_3;
	}//for ( unsigned int index...

	std::cout << "Read from the file OK." << std::endl;

	// Calculating extents...

	// Assume the 1st one is the largest and smallest:
	drawInfo.minX = drawInfo.pVerticesFromFile[0].x;
	drawInfo.minY = drawInfo.pVerticesFromFile[0].y;
	drawInfo.minZ = drawInfo.pVerticesFromFile[0].z;

	drawInfo.maxX = drawInfo.pVerticesFromFile[0].x;
	drawInfo.maxY = drawInfo.pVerticesFromFile[0].y;
	drawInfo.maxZ = drawInfo.pVerticesFromFile[0].z;

	for ( unsigned int index = 0; index != drawInfo.numberOfVertices; index++ )
	{
		if ( drawInfo.pVerticesFromFile[index].x < drawInfo.minX )	{ drawInfo.minX = drawInfo.pVerticesFromFile[index].x; }
		if ( drawInfo.pVerticesFromFile[index].y < drawInfo.minY )	{ drawInfo.minY = drawInfo.pVerticesFromFile[index].y; }
		if ( drawInfo.pVerticesFromFile[index].z < drawInfo.minZ )	{ drawInfo.minZ = drawInfo.pVerticesFromFile[index].z; }

		if ( drawInfo.pVerticesFromFile[index].x > drawInfo.maxX )	{ drawInfo.maxX = drawInfo.pVerticesFromFile[index].x; }
		if ( drawInfo.pVerticesFromFile[index].y > drawInfo.maxY )	{ drawInfo.maxY = drawInfo.pVerticesFromFile[index].y; }
		if ( drawInfo.pVerticesFromFile[index].z > drawInfo.maxZ )	{ drawInfo.maxZ = drawInfo.pVerticesFromFile[index].z; }
	}//for ( unsigned int index...

	drawInfo.extentX = drawInfo.maxX - drawInfo.minX;
	drawInfo.extentY = drawInfo.maxY - drawInfo.minY;
	drawInfo.extentZ = drawInfo.maxZ - drawInfo.minZ;

	drawInfo.maxExtent = drawInfo.extentX;
	if ( drawInfo.maxExtent < drawInfo.extentY )	{	drawInfo.maxExtent = drawInfo.extentY;	}
	if ( drawInfo.maxExtent < drawInfo.extentZ )	{	drawInfo.maxExtent = drawInfo.extentZ;	}

	return true;
}



bool cVAOMeshManager::m_LoadDrawInfo_Into_VAO( 
	sModelDrawInfo &drawInfo, 
	unsigned int shaderProgramID )
{
	//sVert_xyz_rgb* pVertices = new sVert_xyz_rgb[g_numberOfVertices];
//	drawInfo.pVerticesToVBO = new sVert_xyz_rgb[drawInfo.numberOfVertices];
//	drawInfo.pVerticesToVBO = new sVert_xyz_rgb_Nxyz[drawInfo.numberOfVertices];
	drawInfo.pVerticesToVBO = new sVert_xyz_rgb_Nxyz_uv2[drawInfo.numberOfVertices];
	
	// Copy the data from the PLY format to the vertex buffer format
	for ( unsigned int index = 0; index != drawInfo.numberOfVertices; index++ )
	{
		drawInfo.pVerticesToVBO[index].x = drawInfo.pVerticesFromFile[index].x;
		drawInfo.pVerticesToVBO[index].y = drawInfo.pVerticesFromFile[index].y;
		drawInfo.pVerticesToVBO[index].z = drawInfo.pVerticesFromFile[index].z;

		// Normalize the normal (make it 1.0f unit length)
		glm::vec3 normal = glm::vec3( drawInfo.pVerticesFromFile[index].nx, 
									  drawInfo.pVerticesFromFile[index].ny,
									  drawInfo.pVerticesFromFile[index].nz );
		glm::normalize( normal );

		drawInfo.pVerticesToVBO[index].nx = normal.x;
		drawInfo.pVerticesToVBO[index].ny = normal.y;
		drawInfo.pVerticesToVBO[index].nz = normal.z;

		// Set all the colours to white (1,1,1) for now
		drawInfo.pVerticesToVBO[index].r = 1.0f;
		drawInfo.pVerticesToVBO[index].g = 1.0f;
		drawInfo.pVerticesToVBO[index].b = 1.0f;

		// Set the UV values, also
		drawInfo.pVerticesToVBO[index].u0 = drawInfo.pVerticesFromFile[index].u;
		drawInfo.pVerticesToVBO[index].v0 = drawInfo.pVerticesFromFile[index].v;
		// There might be a 2nd set of UV coordinates, but not now
		drawInfo.pVerticesToVBO[index].u1 = 0.0f;
		drawInfo.pVerticesToVBO[index].v1 = 0.0f;
	}


	// Creates a VAO (Vertex Array Object)
	glGenVertexArrays( 1, &(drawInfo.VAO_ID) );
	glBindVertexArray( drawInfo.VAO_ID );

	
	// Allocate a buffer and copy
    //GLuint vertex_buffer;

	// Just to confuse you, this is called VBO (Vertex Buffer Object)
	// NOTE: OpenGL error checks have been omitted for brevity
    glGenBuffers(1, &(drawInfo.vertexBufferID) );			// vertex_buffer
	// "Vertex" buffer
	// - Sets the "type" of buffer
	// - Makes it the 'current' buffer
    glBindBuffer(GL_ARRAY_BUFFER, drawInfo.vertexBufferID);

	//unsigned int vertexBufferSizeInBytes = 
	//					sizeof( sVert_xyz_rgb ) * drawInfo.numberOfVertices;
	//unsigned int vertexBufferSizeInBytes = 
	//					sizeof( sVert_xyz_rgb_Nxyz ) * drawInfo.numberOfVertices;
	unsigned int vertexBufferSizeInBytes = 
						sizeof( sVert_xyz_rgb_Nxyz_uv2 ) * drawInfo.numberOfVertices;
	
//	vector<sVert_xyz_rgb> vecVerticesON_THE_CPU;
//	sVert_xyz_rgb* pVerticesToVBO = new sVert_xyz_rgb[ARRAYSIZE]

	if ( drawInfo.bVertexBufferIsDynamic )
	{	
		// CAN update the vertex information (later)
		glBufferData( GL_ARRAY_BUFFER, 
					  vertexBufferSizeInBytes,		// sizeof(vertices), 
					  drawInfo.pVerticesToVBO,			// vertices, 
					  GL_DYNAMIC_DRAW);	//*************
	}
	else
	{
		// DON'T INDEND to update the vertex buffer info (faster)
		glBufferData( GL_ARRAY_BUFFER, 
					  vertexBufferSizeInBytes,	// sizeof(vertices), 
					  drawInfo.pVerticesToVBO,			// vertices, 
					  GL_STATIC_DRAW);	//*************
	}

	// Create the index buffer...

	// Three indices (of vertices) per triangle
	drawInfo.numberOfIndices = drawInfo.numberOfTriangles * 3;
	drawInfo.pIndices = new unsigned int[drawInfo.numberOfIndices];
	memset( drawInfo.pIndices, 0, sizeof(unsigned int) * drawInfo.numberOfIndices );


	// Load the index buffer...
	unsigned int indexIndex = 0;
	for ( unsigned int triIndex = 0; triIndex != drawInfo.numberOfTriangles; 
		  triIndex++, indexIndex += 3 )
	{
		drawInfo.pIndices[indexIndex + 0] = drawInfo.pTriangles[triIndex].vertex_index_1;
		drawInfo.pIndices[indexIndex + 1] = drawInfo.pTriangles[triIndex].vertex_index_2;
		drawInfo.pIndices[indexIndex + 2] = drawInfo.pTriangles[triIndex].vertex_index_3;
	}


    glGenBuffers(1, &(drawInfo.indexBufferID) );			// vertex_buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.indexBufferID);

	unsigned int indexBufferSizeInBytes = sizeof(unsigned int) * drawInfo.numberOfIndices;

    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 
				  indexBufferSizeInBytes,	// sizeof(vertices), 
				  drawInfo.pIndices,			// vertices, 
				  GL_STATIC_DRAW);

	// Set the vertex layout

    GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPosition");
    GLint vcol_location = glGetAttribLocation(shaderProgramID, "vColour");
    GLint vnorm_location = glGetAttribLocation(shaderProgramID, "vNormal");
    GLint vUVx2_location = glGetAttribLocation(shaderProgramID, "vUV_x2");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer( vpos_location,	// "vPosition"
						   3,				//  vec3 (xyz)
						   GL_FLOAT, 
						   GL_FALSE,		// DON'T "normalize"
                           sizeof(sVert_xyz_rgb_Nxyz_uv2),	// size in bytes //sizeof(float) * 9,		// was 6 
						   (void*) offsetof( sVert_xyz_rgb_Nxyz_uv2, x ) );
//						   (void*) 0);

    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location,		// cColour
						   3,					// vec3 (rgb)
						   GL_FLOAT, 
						   GL_FALSE,
                           sizeof(sVert_xyz_rgb_Nxyz_uv2),	// size in bytes //sizeof(float) * 9,		// was 6 
						   (void*) offsetof( sVert_xyz_rgb_Nxyz_uv2, r ) );
//						   (void*) (sizeof(float) * 3));

	// Now add the normal vertex attribute
    glEnableVertexAttribArray(vnorm_location);
    glVertexAttribPointer(vnorm_location,		// 
						   3,					// vec3 (xyz)
						   GL_FLOAT, 
						   GL_FALSE,
                           sizeof(sVert_xyz_rgb_Nxyz_uv2),	//   9  size in bytes
						   (void*) offsetof( sVert_xyz_rgb_Nxyz_uv2, nx ) );
	
	// Now add the normal vertex attribute
    glEnableVertexAttribArray(vUVx2_location);
    glVertexAttribPointer(vUVx2_location,		// 
						   4,					// vec4 vUV_x2;
						   GL_FLOAT, 
						   GL_FALSE,
                           sizeof(sVert_xyz_rgb_Nxyz_uv2),	//   float u0, v0, u1, v1;
						   (void*) offsetof( sVert_xyz_rgb_Nxyz_uv2, u0 ) );

	// Get rid of everything we don't need.

	// Set the "current" VAO to nothing.
	glBindVertexArray( 0 );

    glDisableVertexAttribArray(vpos_location);
    glDisableVertexAttribArray(vcol_location);
    glDisableVertexAttribArray(vnorm_location);
    glDisableVertexAttribArray(vUVx2_location);

	glBindBuffer(GL_ARRAY_BUFFER, 0 );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	return true;
}

void cVAOMeshManager::m_AppendTextToLastError( std::string text, bool addNewLineBefore /*=true*/ )
{
	std::stringstream ssError;
	ssError.str() = this->m_lastErrorString;

	if ( addNewLineBefore )
	{
		ssError << std::endl;
	}
	ssError << text;

	this->m_lastErrorString = ssError.str();

	return;
}

std::string cVAOMeshManager::GetLastError(bool bAndClear /*=true*/)
{
	std::string errorText = this->m_lastErrorString;
	if ( bAndClear )
	{
		this->m_lastErrorString = "";
	}
	return errorText;
}

void cVAOMeshManager::ShutDown(void)
{

	// TODO: Clear the map

	return;
}

void cVAOMeshManager::UpdateModelVertexInformation( sModelDrawInfo &drawInfo )
{
	glBindBuffer(GL_ARRAY_BUFFER, drawInfo.vertexBufferID);

	//unsigned int vertexBufferSizeInBytes = 
	//					sizeof( sVert_xyz_rgb ) * drawInfo.numberOfVertices;
	//unsigned int vertexBufferSizeInBytes = 
	//					sizeof( sVert_xyz_rgb_Nxyz ) * drawInfo.numberOfVertices;
	unsigned int vertexBufferSizeInBytes = 
						sizeof( sVert_xyz_rgb_Nxyz_uv2 ) * drawInfo.numberOfVertices;
	
	// ASSUME it's a dynmaic buffer
//	if ( drawInfo.bVertexBufferIsDynamic )
//	{	
		// CAN update the vertex information (later)
		glBufferData( GL_ARRAY_BUFFER, 
					  vertexBufferSizeInBytes,		// sizeof(vertices), 
					  drawInfo.pVerticesToVBO,			// vertices, 
					  GL_DYNAMIC_DRAW);	//*************
//	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);	// "un-bind" the buffer

	return;
}


