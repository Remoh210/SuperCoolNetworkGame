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
bool LoadPlyFileData( std::string fileName )
{
	// Open the file that you asked.
	std::ifstream theFile( fileName.c_str() );	

	// if ( theFile.is_open() == false )
	if ( ! theFile.is_open() )			// More "c" or "C++" ish
	{
		std::cout << "Didn't open file" << std::endl;
		return false;
	}
	
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
	theFile >> g_numberOfVertices;

	std::cout << "vertices: " << g_numberOfVertices << std::endl;

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
	theFile >> g_numberOfTriangles;
	std::cout << "triangles: " << g_numberOfTriangles << std::endl;

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
	g_pArrayVert = new sPlyVertex[g_numberOfVertices];	// HEAP

//	ZeroMemory(); win32
	// C call... (clears memory to all zeros)
	memset( g_pArrayVert, 0, sizeof( sPlyVertex ) * g_numberOfVertices );

	// Read the vertex data into the array
	for ( unsigned int index = 0; index != g_numberOfVertices; index++ )
	{
		theFile >> g_pArrayVert[index].x;
		theFile >> g_pArrayVert[index].y;
		theFile >> g_pArrayVert[index].z;

//		theFile >> g_pArrayVert[index].;
	}

	// Same with triangles

//	sPlyTriangle* pArrayTris = new sPlyTriangle[numberOfTriangles];	// HEAP
	g_pArrayTris = new sPlyTriangle[g_numberOfTriangles];	// HEAP

	memset( g_pArrayTris, 0, sizeof( sPlyTriangle ) * g_numberOfTriangles );

	int TossThisAway = 0;
	for ( unsigned int index = 0; index !=  g_numberOfTriangles; index++ )
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
