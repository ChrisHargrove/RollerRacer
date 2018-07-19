#include "RadioButton.h"

#include <GLM/gtc/matrix_transform.hpp>


RadioButton::RadioButton(glm::vec2 position, glm::vec2 size, Texture* textureA, Texture* textureB, std::string shader) :
	Button(position, size, textureA, shader)
{
	_ButtonOnSprite = new Sprite(size, 1, 1, textureB, shader);
	_ButtonOnSprite->SetSpriteSheetLocation();
	_IsOn = false;
}


RadioButton::~RadioButton()
{
	delete _ButtonOnSprite;
}

void RadioButton::Update(float delta)
{
}

void RadioButton::Render()
{
	ShaderManager::Instance()->GetShader(_Shader)->SetVec3("color", glm::vec3(1.0, 1.0, 1.0));
	ShaderManager::Instance()->GetShader(_Shader)->SetBool("RenderingText", false);
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(_Position, 0));
	ShaderManager::Instance()->GetShader(_Shader)->SetMat4("model", model);
	if (_IsOn) {
		_ButtonOnSprite->Render();
	}
	else {
		_ButtonBack->Render();
	}
}

void RadioButton::CallBack()
{
	if (_IsOn) {
		_OtherCallBack();
		_IsOn = false;
	}
	else {
		_CallBack();
		_IsOn = true;
	}
}

void RadioButton::SetOtherCallBack(std::function<void()> callback)
{
	_OtherCallBack = callback;
}
