#pragma once

#include <string>

#include "Sprite.h"
#include "BVehicle.h"

class Speedometer
{
public:
	Speedometer(glm::vec2 position, std::string shader);
	~Speedometer();

	void Update(float delta);
	void Render();

	void SetTarget(BVehicle* target);
	void SetPosition(glm::vec2 position);

	glm::vec2 GetSize();

private:
	std::string _Shader;

	glm::vec2 _Position;
	glm::vec2 _NeedlePos;
	float _NeedleAngle = -180.0f;

	Sprite * _Dial;
	Sprite* _Needle;

	BVehicle* _TargetVehicle;
};

