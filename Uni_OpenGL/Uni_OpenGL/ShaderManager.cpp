////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "ShaderManager.h"
#include "LogManager.h"

////////////////////////////////////////////////////////////
// Static Variables
////////////////////////////////////////////////////////////
ShaderManager ShaderManager::_Instance;

////////////////////////////////////////////////////////////
ShaderManager::ShaderManager() :
	_CurrentShader("")
{
}

////////////////////////////////////////////////////////////
ShaderManager::~ShaderManager()
{
	//loop through all shaders in map, and delete all shader
	//pointers.
	for (auto const& s : _Shaders) {
		delete s.second;
	}
	//clear the map once all shaders are deleted.
	_Shaders.clear();
}

////////////////////////////////////////////////////////////
void ShaderManager::AddShader(std::string Key, std::string FileName)
{
	//check to see if shader key already exists, if not then add new shader.
	auto search = _Shaders.find(Key);
	if (search != _Shaders.end()) {
		LogManager::Instance()->LogWarning("This Shader Name already exists! Choose a new one!...");
	}
	else {
		Shader* newShader = new Shader(FileName);
		_Shaders.emplace(std::make_pair(Key, newShader));
	}
}

////////////////////////////////////////////////////////////
void ShaderManager::UseShader(std::string Key)
{
	//check to see if shader key already exists, if not then throw error message.
	auto search = _Shaders.find(Key);
	if (search != _Shaders.end()) {
		_CurrentShader = Key;
		_Shaders.at(_CurrentShader)->Use();
	}
	else {
		LogManager::Instance()->LogWarning("This Shader does not exist!...");
	}
}

////////////////////////////////////////////////////////////
Shader* ShaderManager::GetShader(std::string Key)
{
	if (_CurrentShader != Key) {
		UseShader(Key);
	}
	//check to see if shader key already exists, if not then throw error message.
	auto search = _Shaders.find(Key);
	if (search != _Shaders.end()) {
		return _Shaders.at(Key);
	}
	else {
		LogManager::Instance()->LogWarning("This Shader does not exist!...");
	}
}
