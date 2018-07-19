#include "Speedometer.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "MathUtil.h"

#include <GLM/glm.hpp>


Speedometer::Speedometer(glm::vec2 position, std::string shader)
{
	_Position = position;
	_Shader = shader;
	_Dial = new Sprite(glm::vec2(300, 300), 1, 1, ResourceManager::Instance()->GetTexture("SpeedoDial"), _Shader);
	_Dial->SetSpriteSheetLocation();
	_Needle = new Sprite(glm::vec2(150, 8), 1, 1, ResourceManager::Instance()->GetTexture("SpeedoNeedle"), _Shader);
	_Needle->SetSpriteSheetLocation();
}

Speedometer::~Speedometer()
{
	delete _Dial;
	delete _Needle;
}

void Speedometer::Update(float delta)
{
	if (_TargetVehicle != nullptr) {
		_NeedleAngle = mapRange(_TargetVehicle->GetCurrentSpeedKmHour(), 0, 180, -180, 47);
	}
}

void Speedometer::Render()
{
	ShaderManager::Instance()->GetShader(_Shader)->SetVec3("color", glm::vec3(1.0, 1.0, 1.0));
	ShaderManager::Instance()->GetShader(_Shader)->SetBool("RenderingText", false);
	ShaderManager::Instance()->GetShader(_Shader)->SetBool("RenderingDepth", false);
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(_Position.x, _Position.y, 0));
	ShaderManager::Instance()->GetShader(_Shader)->SetMat4("model", model);
	_Dial->Render();
	model = glm::translate(model, glm::vec3((_Dial->GetSize().x * 0.5f), (_Dial->GetSize().y * 0.5f) - (_Needle->GetSize().y * 0.5f), 0));
	model = glm::rotate(model, -glm::radians(_NeedleAngle), glm::vec3(0,0,1));
	ShaderManager::Instance()->GetShader(_Shader)->SetMat4("model", model);
	_Needle->Render();
}

void Speedometer::SetTarget(BVehicle * target)
{
	_TargetVehicle = target;
}

void Speedometer::SetPosition(glm::vec2 position)
{
	_Position = position;
}

glm::vec2 Speedometer::GetSize()
{
	return _Dial->GetSize();
}
