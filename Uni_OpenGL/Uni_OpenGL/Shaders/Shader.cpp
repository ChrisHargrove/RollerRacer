#include "Shader.h"
#include "..\LogManager.h"

#include <GLEW\glew.h>

Shader::Shader(const std::string FilePath)
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
		vShaderFile.open(FilePath + ".vert");
		fShaderFile.open(FilePath + ".frag");
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

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string & Name, bool Value) const
{
	glUniform1i(glGetUniformLocation(ID, Name.c_str()), (int)Value);
}

void Shader::SetInt(const std::string & Name, int Value) const
{
	glUniform1i(glGetUniformLocation(ID, Name.c_str()), Value);
}

void Shader::SetFloat(const std::string & Name, float Value) const
{
	glUniform1f(glGetUniformLocation(ID, Name.c_str()), Value);
}

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
