#include "Button.h"
#include <GLM/gtc/matrix_transform.hpp>

#include "ScreenManager.h"

#include <iostream>

Button::Button(glm::vec2 position, glm::vec2 size, Texture* texture, std::string shader)
{
	_Shader = shader;
	_Position = position;
	_Size = size;
	_ButtonBack = new Sprite(size, 1, 1, texture, _Shader);
	_ButtonBack->SetSpriteSheetLocation();
}

Button::~Button()
{
	delete _ButtonBack;
}

void Button::Update(float delta)
{
}

void Button::Render()
{
	ShaderManager::Instance()->GetShader(_Shader)->SetVec3("color", glm::vec3(1.0, 1.0, 1.0));
	ShaderManager::Instance()->GetShader(_Shader)->SetBool("RenderingText", false);
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(_Position, 0));
	ShaderManager::Instance()->GetShader(_Shader)->SetMat4("model", model);
	_ButtonBack->Render();
}

void Button::SetButtonText(std::string text)
{
	_ButtonText = text;
}

void Button::SetButtonText(char text)
{
	std::string temp;
	temp.push_back(text);
	_ButtonText = temp;
}

glm::vec2 Button::GetPosition()
{
	return _Position;
}

glm::vec2 Button::GetSize()
{
	return _Size;
}

void Button::CallBack()
{
	_CallBack();
}

void Button::SetCallBack(std::function<void()> callback)
{
	_CallBack = callback;
}

bool Button::IsColliding(glm::vec2 mousePos)
{
	glm::vec2 screenSize = ScreenManager::Instance()->GetSize();
	if (mousePos.x > _Position.x && mousePos.x < _Position.x + _Size.x &&
		mousePos.y <  (screenSize.y - _Position.y) && mousePos.y > (screenSize.y - (_Position.y + _Size.y))) {
		return true;
	}
	return false;
}
