#include "Shader.h"
#include "LogManager.h"

#include <GLEW\glew.h>

Shader::Shader(const std::string vertexPath, const std::string fragmentPath)
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
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
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
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::string logString(infoLog);
		LogManager::Instance()->LogError("Shader::Vertex::Compilation Failed! :: " + logString);
	};
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::string logString(infoLog);
		LogManager::Instance()->LogError("Shader::Vertex::Compilation Failed! :: " + logString);
	};

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::string logString(infoLog);
		LogManager::Instance()->LogError("Shader::Vertex::Linking Failed! :: " + logString);
	}

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	//I'm A Penis
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

void Shader::SetFloat(const std::string & Name, int Value) const
{
	glUniform1f(glGetUniformLocation(ID, Name.c_str()), Value);
}
