#pragma once

#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>

#include <GLM\gtx\rotate_vector.hpp>

class GameObject
{
public:
	GameObject(glm::vec3 position) {
		_Position = position;
		_ModelMatrix = glm::mat4(1.0);
		_Up = DEFAULT_UP;
		_Right = DEFAULT_RIGHT;
		_Forward = DEFAULT_FORWARD;
	}
	virtual ~GameObject() {}

	virtual void Input() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	glm::mat4& GetModelMatrix() { return _ModelMatrix; }
	glm::vec3& GetPosition() { return _Position; }

	glm::vec3& GetDirection() { return _Forward; }

	void Scale(glm::vec3 scale) { _ModelMatrix = glm::scale(_ModelMatrix, scale); }
	void Scale(float x, float y, float z) { _ModelMatrix = glm::scale(_ModelMatrix, glm::vec3(x, y, z)); }
	void Scale(float scale) { _ModelMatrix = glm::scale(_ModelMatrix, glm::vec3(scale, scale, scale)); }

	void Translate(glm::vec3 translate) { 
		_ModelMatrix = glm::translate(_ModelMatrix, translate);
		_Position = glm::vec3(_ModelMatrix[3][0], _ModelMatrix[3][1], _ModelMatrix[3][2]);
	}
	void Translate(float x, float y, float z) { 
		_ModelMatrix = glm::translate(_ModelMatrix, glm::vec3(x, y, z)); 
		_Position = glm::vec3(_ModelMatrix[3][0], _ModelMatrix[3][1], _ModelMatrix[3][2]);
	}
	void Rotate(float angle, glm::vec3 axis) { 
		_ModelMatrix = glm::rotate(_ModelMatrix, angle, axis); 
        _Up = glm::rotate(_Up, angle, axis);
        _Right = glm::rotate(_Right, angle, axis);
        _Forward = glm::rotate(_Forward, angle, axis);
	}

protected:
	const glm::vec3 DEFAULT_UP = glm::vec3(0, 1, 0);
	const glm::vec3 DEFAULT_RIGHT = glm::vec3(1, 0, 0);
	const glm::vec3 DEFAULT_FORWARD = glm::vec3(0, 0, -1);

	glm::vec3 _Up;
	glm::vec3 _Right;
	glm::vec3 _Forward;
	glm::vec3 _Position;

	glm::mat4 _ModelMatrix;
};

