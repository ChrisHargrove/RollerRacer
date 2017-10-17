#pragma once

#include <string>
#include <fstream>
#include <sstream>

class Shader
{
public:
	unsigned int ID;

	Shader(const std::string vertexPath, const std::string fragmentPath);

	void Use();

	void SetBool(const std::string &Name, bool Value) const;
	void SetInt(const std::string &Name, int Value) const;
	void SetFloat(const std::string &Name, int Value) const;
};

