//
//
//// OpenGLMeshLoader.cpp
//// Take a model file array and upload it to the GPU
//
//#include "globalStuff.h" 
//
////#include "sPlyVertex.h"
////#include "sPlyTriangle.h"
//
//
//// This is based on what the shader is expecting
//struct sVert_xyz_rgb
//{
//	sVert_xyz_rgb() : 
//		x(0.0f), y(0.0f), z(0.0f), 
//		r(0.0f), g(0.0f), b(0.0f) {};
//	float x, y, z;		// vec3    
//	float r, g, b;		// vec3 
//};
//
//
////sVert_xyz_rgb vertices[6] =				
////{	//   x      y    z     r      g      b
////    { -0.6f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f },	// 1
////    {  0.6f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f },	// 2
////    {  0.0f,  0.6f, 0.0f, 0.0f, 0.0f, 1.0f }, // 3
////
////    { -1.6f, -1.4f, 0.0f, 0.0f, 0.0f, 1.0f },	// 0
////    {  1.6f, -1.4f, 0.0f, 0.0f, 0.0f, 1.0f },	// 1
////    {  1.0f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f }	// 5
////};
//
//// Takes an arran of vertices and indices
////	and copies the data to the GPU
//bool LoadMeshIntoGPUBuffer(void)
//{
//	// Allocate another temp array for the vertex buffer
////	sPlyTriangle* pArrayTris = new sPlyTriangle[numberOfTriangles];	// HEAP
//
//	sVert_xyz_rgb* pVertices = new sVert_xyz_rgb[g_numberOfVertices];
//	
//	// Copy the data from the PLY format to the vertex buffer format
//	for ( unsigned int index = 0; index != g_numberOfVertices; index++ )
//	{
//		pVertices[index].x = g_pArrayVert[index].x;
//		pVertices[index].y = g_pArrayVert[index].y;
//		pVertices[index].z = g_pArrayVert[index].z;
//
//		// Set all the colours to white (1,1,1) for now
//		pVertices[index].r = 1.0f;
//		pVertices[index].g = 1.0f;
//		pVertices[index].b = 1.0f;
//	}
//
//
//	// Allocate a buffer and copy
//    GLuint vertex_buffer;
//
//	// NOTE: OpenGL error checks have been omitted for brevity
//    glGenBuffers(1, &vertex_buffer);
//	// "Vertex" buffer
//	// - Sets the "type" of buffer
//	// - Makes it the 'current' buffer
//    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//
//	unsigned int bufferSizeInBytes = 
//						sizeof( sVert_xyz_rgb ) * g_numberOfVertices;
//
//    glBufferData( GL_ARRAY_BUFFER, 
//				  bufferSizeInBytes,	// sizeof(vertices), 
//				  pVertices,			// vertices, 
//				  GL_STATIC_DRAW);
//
//	// Get rid of everything we don't need.
//
//
//	return true;
//}
//
//// no extern here
//unsigned int g_UnrolledNumberOfVertices = 0;
//
//// We'll never use this after today, BUT it is important for now
//// Takes an arran of vertices and indices
////	and copies the data to the GPU
//bool Load_And_Unroll_MeshIntoGPUBuffer(void)
//{
//	// Allocate another temp array for the vertex buffer
//
//	// Figure out how many vertices there WOULD BE if the 
//	//	vertex buffer was based on the triangle ('face') list.
//
//	// extern sPlyVertex*	g_pArrayVert;	Vertices are here
//	// extern sPlyTriangle*	g_pArrayTris;	Triangles are here
//
//	// Each triangle has 3 vertices...
//	g_UnrolledNumberOfVertices = g_numberOfTriangles * 3;
//
//	//	sVert_xyz_rgb* pVertices = new sVert_xyz_rgb[g_numberOfVertices];
//	sVert_xyz_rgb* pVertices = new sVert_xyz_rgb[g_UnrolledNumberOfVertices];
//
//	unsigned int vertIndex = 0;	// Index into the VERTEX array
//
//	for ( unsigned int triIndex = 0; 
//		  triIndex != g_numberOfTriangles; 
//		  triIndex += 1,			// same as ++
//		  vertIndex += 3)			
//	{
//		//sPlyVertex curVert1 = g_pArrayVert[ g_pArrayTris[triIndex].vertex_index_1 ];
//
//		unsigned vert1Index = g_pArrayTris[triIndex].vertex_index_1;
//		sPlyVertex curVert1 = g_pArrayVert[vert1Index];
//		
//		unsigned vert2Index = g_pArrayTris[triIndex].vertex_index_2;
//		sPlyVertex curVert2 = g_pArrayVert[vert2Index];
//
//		unsigned vert3Index = g_pArrayTris[triIndex].vertex_index_3;
//		sPlyVertex curVert3 = g_pArrayVert[vert3Index];
//
//		// Copy the actual vertex data... (there's 3 of them)
//
//		pVertices[vertIndex + 0].x = curVert1.x;
//		pVertices[vertIndex + 0].y = curVert1.y;
//		pVertices[vertIndex + 0].z = curVert1.z;
//		pVertices[vertIndex + 0].r = 1.0f;
//		pVertices[vertIndex + 0].g = 1.0f;
//		pVertices[vertIndex + 0].b = 1.0f;
//		
//		pVertices[vertIndex + 1].x = curVert2.x;
//		pVertices[vertIndex + 1].y = curVert2.y;
//		pVertices[vertIndex + 1].z = curVert2.z;
//		pVertices[vertIndex + 1].r = 1.0f;
//		pVertices[vertIndex + 1].g = 1.0f;
//		pVertices[vertIndex + 1].b = 1.0f;
//		
//		pVertices[vertIndex + 2].x = curVert3.x;
//		pVertices[vertIndex + 2].y = curVert3.y;
//		pVertices[vertIndex + 2].z = curVert3.z;
//		pVertices[vertIndex + 2].r = 1.0f;
//		pVertices[vertIndex + 2].g = 1.0f;
//		pVertices[vertIndex + 2].b = 1.0f;	
//	}
//
//
//
//	
//	// Copy the data from the PLY format to the vertex buffer format
//	//for ( unsigned int index = 0; index != g_numberOfVertices; index++ )
//	//{
//	//	pVertices[index].x = g_pArrayVert[index].x;
//	//	pVertices[index].y = g_pArrayVert[index].y;
//	//	pVertices[index].z = g_pArrayVert[index].z;
//
//	//	// Set all the colours to white (1,1,1) for now
//	//	pVertices[index].r = 1.0f;
//	//	pVertices[index].g = 1.0f;
//	//	pVertices[index].b = 1.0f;
//	//}
//
//
//	// Allocate a buffer and copy
//    GLuint vertex_buffer;
//
//	// NOTE: OpenGL error checks have been omitted for brevity
//    glGenBuffers(1, &vertex_buffer);
//	// "Vertex" buffer
//	// - Sets the "type" of buffer
//	// - Makes it the 'current' buffer
//    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//
//	//unsigned int bufferSizeInBytes = 
//	//					sizeof( sVert_xyz_rgb ) * g_numberOfVertices;
//	unsigned int bufferSizeInBytes = 
//						sizeof( sVert_xyz_rgb ) * g_UnrolledNumberOfVertices;
//
//    glBufferData( GL_ARRAY_BUFFER, 
//				  bufferSizeInBytes,	// sizeof(vertices), 
//				  pVertices,			// vertices, 
//				  GL_STATIC_DRAW);
//
//	// Get rid of everything we don't need.
//
//
//	return true;
//}