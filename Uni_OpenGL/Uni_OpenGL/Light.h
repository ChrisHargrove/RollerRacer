#pragma once

#include <GLM\glm.hpp>
#include <string>

class DirectionalLight
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~DirectionalLight();

	void SetDirection(glm::vec3 direction);
	void SetAmbient(glm::vec3 color);
	void SetDiffuse(glm::vec3 color);
	void SetSpecular(glm::vec3 color);

	void SendToShader(std::string shader);

private:
	glm::vec3	_Ambient;
	glm::vec3	_Diffuse;
	glm::vec3	_Specular;
	glm::vec3	_Direction;
};

class PointLight
{
public:
	PointLight();
	PointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~PointLight();

	void SetPosition(glm::vec3 position);
	void SetAmbient(glm::vec3 color);
	void SetDiffuse(glm::vec3 color);
	void SetSpecular(glm::vec3 color);

	void SetConstant(float value);
	void SetLinear(float value);
	void SetQuadratic(float value);

	void SendToShader(int pointNum, std::string shader);

private:
	glm::vec3	_Ambient;
	glm::vec3	_Diffuse;
	glm::vec3	_Specular;
	glm::vec3	_Position;

	float	_Constant;
	float	_Linear;
	float	_Quadratic;
};

