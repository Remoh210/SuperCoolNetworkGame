#ifndef _cShaderManager_HG_
#define _cShaderManager_HG_

#include <string>
#include <vector>
#include <map>

class cShaderManager
{
public:
	class cShader {
	public:
		cShader();
		~cShader();
		enum eShaderType
		{
			VERTEX_SHADER,
			FRAGMENT_SHADER,
			GEOMETRY_SHADER,
			TESSELATION_SHADER,
			COMPUTE_SHADER,
			UNKNOWN
		};
		eShaderType shaderType;
		std::string getShaderTypeString(void);

		unsigned int ID;	// or "name" from OpenGL
		std::vector<std::string> vecSource;
		void parseStringIntoMultiLine(std::string singleLineSouce);
		bool bSourceIsMultiLine;
		std::string fileName;

	};

	class cShaderProgram {
	public:
		cShaderProgram() : ID(0) {};
		~cShaderProgram() {};
		unsigned int ID;	// ID from OpenGL (calls it a "name")
		std::string friendlyName;	// We give it this name

		// TODO: For the students to do, because it's FUN, FUN, FUN
		void LoadUniformLocations(std::string uniformName);
		std::map< std::string /*name of uniform variable*/,
			int /* uniform location ID */ >
			mapUniformName_to_UniformLocation;
		// Returns -1 (just like OpenGL) if NOT found
		int getUniformID_From_Name(std::string name);
		// Look up the uniform location and save it.
		bool LoadUniformLocation(std::string variableName);

	};

	cShaderManager();
	~cShaderManager();

	bool useShaderProgram(unsigned int ID);
	bool useShaderProgram(std::string friendlyName);

	bool createProgramFromFile(std::string friendlyName,
		cShader &vertexShad,
		cShader &fragShader);

	bool createProgramFromFile(std::string friendlyName,
		cShader &vertexShad,
		cShader &geomShad,
		cShader &fragShader);

	bool createProgramFromSource(std::string friendlyName,
		cShader &vertexShad,
		cShader &fragShader);

	bool createProgramFromSource(std::string friendlyName,
		cShader &vertexShad,
		cShader &geomShad,
		cShader &fragShader);


	void setBasePath(std::string basepath);
	unsigned int getIDFromFriendlyName(std::string friendlyName);

	// Used to load the uniforms. Returns NULL if not found.
	cShaderProgram* pGetShaderProgramFromFriendlyName(std::string friendlyName);
	cShaderProgram* pGetShaderProgramFromID(int shaderID);


	// Clears last error
	std::string getLastError(void);
private:
	// Returns an empty string if it didn't work
	bool m_loadSourceFromFile(cShader &shader);
	std::string m_basepath;

	bool m_compileShaderFromSource(cShader &shader, std::string &error);
	// returns false if no error
	bool m_wasThereACompileError(unsigned int shaderID, std::string &errorText);
	bool m_wasThereALinkError(unsigned int progID, std::string &errorText);

	std::string m_lastError;

	std::map< unsigned int /*ID*/, cShaderProgram > m_ID_to_Shader;
	std::map< std::string /*name*/, unsigned int /*ID*/ > m_name_to_ID;
};

#endif