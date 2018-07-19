#pragma once

#include "Sprite.h"
#include "Texture.h"
#include <GLM/glm.hpp>
#include <functional>
#include <string>

class Button
{
public:
	Button(glm::vec2 position, glm::vec2 size, Texture* texture, std::string shader);
	virtual ~Button();

	virtual void Update(float delta);
	virtual void Render();

	void SetButtonText(std::string text);
	void SetButtonText(char text);
	std::string GetButtonText() { return _ButtonText; };

	glm::vec2 GetPosition();
	glm::vec2 GetSize();

	virtual void CallBack();
	void SetCallBack(std::function<void()> callback);
	
	bool IsColliding(glm::vec2 mousePos);

protected:
	glm::vec3 _Color;
	std::string _ButtonText;

	std::string _Shader;
	glm::vec2 _Position;
	glm::vec2 _Size;
	Sprite * _ButtonBack;

	std::function<void()> _CallBack;

};

