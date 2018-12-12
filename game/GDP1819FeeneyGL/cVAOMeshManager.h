#ifndef _cVAOMeshManager_HG_
#define _cVAOMeshManager_HG_

#include <string>
#include <map>
#include "sModelDrawInfo.h"

// This class is in charge of:
// - taking a cMesh object (loaded from file)
// - loading that mesh data into a vertex and index buffer, 
//   inside a Vertex Array Object (VAO)
// - looking up the rendering (drawing) information for rendering
// - allowing the mesh objects to be "dynamic" if needed



class cVAOMeshManager
{
public:
	cVAOMeshManager();		// constructor
	~cVAOMeshManager();		// destructor

	// Note: the shader program ID is needed to tie 
	// the buffer to the vertex layout of the shader
	bool LoadModelIntoVAO( sModelDrawInfo &drawInfo,	
						   unsigned int shaderProgramID );

	// Looks up draw info by name (meshFileName)			
	bool FindDrawInfoByModelName( sModelDrawInfo &drawInfo );

	void ShutDown(void);

	std::string GetLastError(bool bClear = true);

	// Update the mesh information, then re-copy to GPU
	// - This will RE-COPY the VERTEX infoarmation into the vertex buffer
	//   (will copy ENTIRE vertex buffer, OVERWRITING what's there)
	void UpdateModelVertexInformation( sModelDrawInfo &drawInfo );

private:

	// LoadPlyFileData()
	bool m_LoadModelFromFile( sModelDrawInfo &drawInfo );

	//LoadMeshIntoGPUBuffer
	bool m_LoadDrawInfo_Into_VAO( sModelDrawInfo &drawInfo, 
								  unsigned int shaderProgramID );
								  

	// This holds the model information
	std::map< std::string /*model name*/, sModelDrawInfo > m_mapModel_to_VAOID;

	std::string m_lastErrorString;
	void m_AppendTextToLastError( std::string text, bool addNewLineBefore = true );
};

#endif		// _cVAOMeshManager_HG_
