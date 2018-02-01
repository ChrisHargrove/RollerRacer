#include "GameObject.h"



GameObject::GameObject()
{
	_ModelMatrix = glm::mat4();
}


GameObject::~GameObject()
{
}

glm::mat4 & GameObject::GetModelMatrix()
{
	return _ModelMatrix;
}
