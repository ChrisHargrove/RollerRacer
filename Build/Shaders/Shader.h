////////////////////////////////////////////////////////////
//
// Shader
// Author - Chris Hargrove
// Date   - 17/10/2017
//
////////////////////////////////////////////////////////////
#ifndef SHADER_H
#define SHADER_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <string>
#include <GLM\glm.hpp>

////////////////////////////////////////////////////////////
/// Engine Shader
/// --Handles the creation of shader objects aswell as
/// --reading vertex and fragment shaders into memory
/// --and compiling them into a program.
////////////////////////////////////////////////////////////
class Shader
{
public:
	////////////////////////////////////////////////////////////
	/// Default Constructor.
	/// --FileName-- Give name of file to open without file extension
	/// or path.
	////////////////////////////////////////////////////////////
	Shader(const std::string FileName);

	////////////////////////////////////////////////////////////
	/// Default Destructor.
	////////////////////////////////////////////////////////////
	~Shader();

	////////////////////////////////////////////////////////////
	/// Sets this Shader to be the current shader to use for 
	/// Rendering on the GPU.
	////////////////////////////////////////////////////////////
	void Use();

	////////////////////////////////////////////////////////////
	/// Utility Function to update all shader matrices at once.
	/// --Model-- The model matrix to pass to shader.
	/// --View-- The view matrix to pass to shader.
	////////////////////////////////////////////////////////////
	void UpdateMatrices(const glm::mat4 &Model, const glm::mat4 &View);

	////////////////////////////////////////////////////////////
	/// Utility Function to update all shader matrices at once.
	/// --Model-- The model matrix to pass to shader.
	/// --View-- The view matrix to pass to shader.
	/// --Projection -- The projection matrix to pass to shader.
	////////////////////////////////////////////////////////////
	void UpdateMatrices(const glm::mat4 &Model, const glm::mat4 &View, const glm::mat4 &Projection);

	////////////////////////////////////////////////////////////
	/// Sets a boolean variable inside the Shader program to a 
	/// specified value.
	/// --Name-- The name of the variable inside the Shader Program
	/// --Value-- The value to set the variable to.
	////////////////////////////////////////////////////////////
	void SetBool(const std::string &Name, bool Value) const;

	////////////////////////////////////////////////////////////
	/// Sets an integer variable inside the Shader program to a 
	/// specified value.
	/// --Name-- The name of the variable inside the Shader Program
	/// --Value-- The value to set the variable to.
	////////////////////////////////////////////////////////////
	void SetInt(const std::string &Name, int Value) const;

	////////////////////////////////////////////////////////////
	/// Sets a float variable inside the Shader program to a 
	/// specified value.
	/// --Name-- The name of the variable inside the Shader Program
	/// --Value-- The value to set the variable to.
	////////////////////////////////////////////////////////////
	void SetFloat(const std::string &Name, float Value) const;

	////////////////////////////////////////////////////////////
	/// Sets a vector 2 variable inside the Shader program to a 
	/// specified value.
	/// --Name-- The name of the variable inside the Shader Program
	/// --Value-- The value to set the variable to.
	////////////////////////////////////////////////////////////
	void SetVec2(const std::string &Name, const glm::vec2 &Value) const;

	////////////////////////////////////////////////////////////
	/// Sets a vector 2 variable inside the Shader program to a 
	/// specified value.
	/// --Name-- The name of the variable inside the Shader Program
	/// --x-- The value to set the variable to.
	/// --y-- The value to set the variable to.
	////////////////////////////////////////////////////////////
	void SetVec2(const std::string &Name, float x, float y) const;

	////////////////////////////////////////////////////////////
	/// Sets a vector 3 variable inside the Shader program to a 
	/// specified value.
	/// --Name-- The name of the variable inside the Shader Program
	/// --Value-- The value to set the variable to.
	////////////////////////////////////////////////////////////
	void SetVec3(const std::string &Name, const glm::vec3 &Value) const;

	////////////////////////////////////////////////////////////
	/// Sets a vector 3 variable inside the Shader program to a 
	/// specified value.
	/// --Name-- The name of the variable inside the Shader Program
	/// --x-- The value to set the variable to.
	/// --y-- The value to set the variable to.
	/// --z-- The value to set the variable to.
	////////////////////////////////////////////////////////////
	void SetVec3(const std::string &Name, float x, float y, float z) const;

	////////////////////////////////////////////////////////////
	/// Sets a vector 4 variable inside the Shader program to a 
	/// specified value.
	/// --Name-- The name of the variable inside the Shader Program
	/// --Value-- The value to set the variable to.
	////////////////////////////////////////////////////////////
	void SetVec4(const std::string &Name, const glm::vec4 &Value) const;

	////////////////////////////////////////////////////////////
	/// Sets a vector 4 variable inside the Shader program to a 
	/// specified value.
	/// --Name-- The name of the variable inside the Shader Program
	/// --x-- The value to set the variable to.
	/// --y-- The value to set the variable to.
	/// --z-- The value to set the variable to.
	/// --w-- The value to set the variable to.
	////////////////////////////////////////////////////////////
	void SetVec4(const std::string &Name, float x, float y, float z, float w);

	////////////////////////////////////////////////////////////
	/// Sets a 2x2 matrix variable inside the Shader program to a 
	/// specified value.
	/// --Name-- The name of the variable inside the Shader Program
	/// --Mat-- The matrix values to set the variables to.
	////////////////////////////////////////////////////////////
	void SetMat2(const std::string &Name, const glm::mat2 &Mat) const;

	////////////////////////////////////////////////////////////
	/// Sets a 3x3 matrix variable inside the Shader program to a 
	/// specified value.
	/// --Name-- The name of the variable inside the Shader Program
	/// --Mat-- The matrix values to set the variables to.
	////////////////////////////////////////////////////////////
	void SetMat3(const std::string &Name, const glm::mat3 &Mat) const;

	////////////////////////////////////////////////////////////
	/// Sets a 4x4 matrix variable inside the Shader program to a 
	/// specified value.
	/// --Name-- The name of the variable inside the Shader Program
	/// --Mat-- The matrix values to set the variables to.
	////////////////////////////////////////////////////////////
	void SetMat4(const std::string &Name, const glm::mat4 &Mat) const;

	////////////////////////////////////////////////////////////
	/// Returns Shader Program ID.
	////////////////////////////////////////////////////////////
	unsigned int GetID() const;

private:
	////////////////////////////////////////////////////////////
	/// Utility function to check for compilation errors
	/// --Shader-- The shader ID hat you want to check.
	/// --Type-- The type of shader that your checking.
	////////////////////////////////////////////////////////////
	bool CheckCompileErrors(unsigned int Shader, std::string Type);

	////////////////////////////////////////////////////////////
	// Member Data
	////////////////////////////////////////////////////////////
	unsigned int ID;	// ID for the shader program in memory
};

#endif