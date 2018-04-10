#include "Light.h"
#include "ShaderManager.h"


DirectionalLight::DirectionalLight()
{
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	_Direction = direction;
	_Ambient = ambient;
	_Diffuse = diffuse;
	_Specular = specular;
}


DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::SetDirection(glm::vec3 direction)
{
	_Direction = direction;
}

void DirectionalLight::SetAmbient(glm::vec3 color)
{
	_Ambient = color;
}

void DirectionalLight::SetDiffuse(glm::vec3 color)
{
	_Diffuse = color;
}

void DirectionalLight::SetSpecular(glm::vec3 color)
{
	_Specular = color;
}

void DirectionalLight::SendToShader(std::string shader)
{
	ShaderManager::Instance()->GetShader(shader)->SetVec3("dirLight.direction", _Direction);
	ShaderManager::Instance()->GetShader(shader)->SetVec3("dirLight.ambient", _Ambient);
	ShaderManager::Instance()->GetShader(shader)->SetVec3("dirLight.diffuse", _Diffuse);
	ShaderManager::Instance()->GetShader(shader)->SetVec3("dirLight.specular", _Specular);

	ShaderManager::Instance()->GetShader(shader)->SetInt("hasDirLight", 1);
}

PointLight::PointLight()
{
}

PointLight::PointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	_Position = position;
	_Ambient = ambient;
	_Diffuse = diffuse;
	_Specular = specular;
	_Constant = constant;
	_Linear = linear;
	_Quadratic = quadratic;
}

PointLight::~PointLight()
{
}

void PointLight::SetPosition(glm::vec3 position)
{
	_Position = position;
}

void PointLight::SetAmbient(glm::vec3 color)
{
	_Ambient = color;
}

void PointLight::SetDiffuse(glm::vec3 color)
{
	_Diffuse = color;
}

void PointLight::SetSpecular(glm::vec3 color)
{
	_Specular = color;
}

void PointLight::SetConstant(float value)
{
	_Constant = value;
}

void PointLight::SetLinear(float value)
{
	_Linear = value;
}

void PointLight::SetQuadratic(float value)
{
	_Quadratic = value;
}

void PointLight::SendToShader(int pointNum, std::string shader)
{
	ShaderManager::Instance()->GetShader(shader)->SetVec3("pointLights[" + std::to_string(pointNum) + "].position", _Position);
	ShaderManager::Instance()->GetShader(shader)->SetVec3("pointLights[" + std::to_string(pointNum) + "].ambient", _Ambient);
	ShaderManager::Instance()->GetShader(shader)->SetVec3("pointLights[" + std::to_string(pointNum) + "].diffuse", _Diffuse);
	ShaderManager::Instance()->GetShader(shader)->SetVec3("pointLights[" + std::to_string(pointNum) + "].specular", _Specular);

	ShaderManager::Instance()->GetShader(shader)->SetFloat("pointLights[" + std::to_string(pointNum) + "].constant", _Constant);
	ShaderManager::Instance()->GetShader(shader)->SetFloat("pointLights[" + std::to_string(pointNum) + "].linear", _Linear);
	ShaderManager::Instance()->GetShader(shader)->SetFloat("pointLights[" + std::to_string(pointNum) + "].quadratic", _Quadratic);

	ShaderManager::Instance()->GetShader(shader)->SetInt("hasPointLight", 1);
	ShaderManager::Instance()->GetShader(shader)->SetInt("numPointLight", pointNum + 1);
}
