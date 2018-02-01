#pragma once

#include <GLM\glm.hpp>

class GameObject
{
public:
	GameObject();
	~GameObject();

	glm::mat4& GetModelMatrix();

private:
	glm::mat4 _ModelMatrix;
};

