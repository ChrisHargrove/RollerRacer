////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Shader.h"
#include <fstream>
#include <sstream>
#include <GLEW\glew.h>
#include "..\LogManager.h"

////////////////////////////////////////////////////////////
Shader::Shader(const std::string FileName)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try	{
		// open files
		vShaderFile.open("Shaders/" + FileName + ".vert");
		fShaderFile.open("Shaders/" + FileName + ".frag");
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		LogManager::Instance()->LogError("Shader::File Not Successfully Read!");
	}
	LogManager::Instance()->LogDebug("Both Shader Files Opened Successfully.");

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	
	if (!CheckCompileErrors(vertex, "VERTEX")) {
		LogManager::Instance()->LogDebug("Vertex Shader Compiled Successfully.");
	}

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	if(!CheckCompileErrors(fragment, "FRAGMENT")) {
		LogManager::Instance()->LogDebug("Fragment Shader Compiled Successfully.");
	}

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	
	if(!CheckCompileErrors(ID, "PROGRAM")) {
		LogManager::Instance()->LogDebug("Shader Program Linked Successfully.");
	}

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);

}

////////////////////////////////////////////////////////////
Shader::~Shader()
{
	glDeleteProgram(ID);
}

////////////////////////////////////////////////////////////
void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::UpdateMatrices(const glm::mat4 & Model, const glm::mat4 & View)
{
	SetMat4("model", Model);
	SetMat4("view", View);
}

void Shader::UpdateMatrices(const glm::mat4 & Model, const glm::mat4 & View, const glm::mat4 & Projection)
{
	SetMat4("model", Model);
	SetMat4("view", View);
	SetMat4("projection", Projection);
}

////////////////////////////////////////////////////////////
void Shader::SetBool(const std::string & Name, bool Value) const
{
	glUniform1i(glGetUniformLocation(ID, Name.c_str()), (int)Value);
}

////////////////////////////////////////////////////////////
void Shader::SetInt(const std::string & Name, int Value) const
{
	glUniform1i(glGetUniformLocation(ID, Name.c_str()), Value);
}

////////////////////////////////////////////////////////////
void Shader::SetFloat(const std::string & Name, float Value) const
{
	glUniform1f(glGetUniformLocation(ID, Name.c_str()), Value);
}

////////////////////////////////////////////////////////////
void Shader::SetVec2(const std::string &Name, const glm::vec2 &Value) const
{
	glUniform2fv(glGetUniformLocation(ID, Name.c_str()), 1, &Value[0]);
}

////////////////////////////////////////////////////////////
void Shader::SetVec2(const std::string &Name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, Name.c_str()), x, y);
}

////////////////////////////////////////////////////////////
void Shader::SetVec3(const std::string &Name, const glm::vec3 &Value) const
{
	glUniform3fv(glGetUniformLocation(ID, Name.c_str()), 1, &Value[0]);
}

////////////////////////////////////////////////////////////
void Shader::SetVec3(const std::string &Name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, Name.c_str()), x, y, z);
}

////////////////////////////////////////////////////////////
void Shader::SetVec4(const std::string &Name, const glm::vec4 &Value) const
{
	glUniform4fv(glGetUniformLocation(ID, Name.c_str()), 1, &Value[0]);
}

////////////////////////////////////////////////////////////
void Shader::SetVec4(const std::string &Name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, Name.c_str()), x, y, z, w);
}

////////////////////////////////////////////////////////////
void Shader::SetMat2(const std::string &Name, const glm::mat2 &Mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, Name.c_str()), 1, GL_FALSE, &Mat[0][0]);
}

////////////////////////////////////////////////////////////
void Shader::SetMat3(const std::string &Name, const glm::mat3 &Mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, Name.c_str()), 1, GL_FALSE, &Mat[0][0]);
}

////////////////////////////////////////////////////////////
void Shader::SetMat4(const std::string &Name, const glm::mat4 &Mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, Name.c_str()), 1, GL_FALSE, &Mat[0][0]);
}

////////////////////////////////////////////////////////////
unsigned int Shader::GetID() const
{
	return ID;
}

////////////////////////////////////////////////////////////
bool Shader::CheckCompileErrors(unsigned int Shader, std::string Type)
{
	int success;
	char infoLog[1024];
	if (Type != "PROGRAM") {
		glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(Shader, 1024, NULL, infoLog);
			std::string logString(infoLog);
			LogManager::Instance()->LogError("Shader Compilation Error of Type:: " + Type + "\n" + logString);
			return true;
		}
	}
	else {
		glGetProgramiv(Shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(Shader, 1024, NULL, infoLog);
			std::string logString(infoLog);
			LogManager::Instance()->LogError("Shader Linking Error of Type:: " + Type + "\n" + logString);
			return true;
		}
	}
	return false;
}
