#pragma once

#include "Cuboid.h"
#include "ShaderManager.h"

#include <vector>
#include <GLM\gtc\matrix_transform.hpp>

class Axis
{
public:
	Axis(float length, std::string ShaderName)
	{
		_Shader = ShaderName;
		_AxisLength = length;
		xPos = new Cuboid(_AxisLength, 0.05, 0.05, _Shader, glm::vec3(1.0, 0.0, 0.0));
		xNeg = new Cuboid(_AxisLength, 0.05, 0.05, _Shader, glm::vec3(0.5, 0.0, 0.0));

		yPos = new Cuboid(0.05, _AxisLength, 0.05, _Shader, glm::vec3(0.0, 1.0, 0.0));
		yNeg = new Cuboid(0.05, _AxisLength, 0.05, _Shader, glm::vec3(0.0, 0.5, 0.0));

		zPos = new Cuboid(0.05, 0.05, _AxisLength, _Shader, glm::vec3(0.0, 0.0, 1.0));
		zNeg = new Cuboid(0.05, 0.05, _AxisLength, _Shader, glm::vec3(0.0, 0.0, 0.5));
	}

	~Axis()
	{
		delete xPos;
		delete xNeg;
		delete yPos;
		delete yNeg;
		delete zPos;
		delete zNeg;
	}

	void Draw()
	{
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(_AxisLength / 2, 0.0, 0.0));
		ShaderManager::Instance()->GetShader(_Shader)->SetMat4("model", model);
		xPos->Draw();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-_AxisLength / 2, 0.0, 0.0));
		ShaderManager::Instance()->GetShader(_Shader)->SetMat4("model", model);
		xNeg->Draw();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0, _AxisLength / 2, 0.0));
		ShaderManager::Instance()->GetShader(_Shader)->SetMat4("model", model);
		yPos->Draw();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0, -_AxisLength / 2, 0.0));
		ShaderManager::Instance()->GetShader(_Shader)->SetMat4("model", model);
		yNeg->Draw();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0, 0.0, _AxisLength / 2));
		ShaderManager::Instance()->GetShader(_Shader)->SetMat4("model", model);
		zPos->Draw();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0, 0.0, -_AxisLength / 2));
		ShaderManager::Instance()->GetShader(_Shader)->SetMat4("model", model);
		zNeg->Draw();
	}

private:
	Cuboid* xPos;	Cuboid* xNeg;
	Cuboid* yPos;	Cuboid* yNeg;
	Cuboid* zPos;	Cuboid* zNeg;
	std::string _Shader;
	float _AxisLength;

};

