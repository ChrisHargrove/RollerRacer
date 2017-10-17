#pragma once

#include <string>
#include <fstream>
#include <sstream>

class Shader
{
public:
	unsigned int ID;

	Shader(const std::string FilePath);

	void Use();

	void SetBool(const std::string &Name, bool Value) const;
	void SetInt(const std::string &Name, int Value) const;
	void SetFloat(const std::string &Name, float Value) const;

private:

	bool CheckCompileErrors(unsigned int Shader, std::string Type);
};

