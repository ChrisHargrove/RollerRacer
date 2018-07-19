#pragma once
#include "Button.h"
class RadioButton :
	public Button
{
public:
	RadioButton(glm::vec2 position, glm::vec2 size, Texture* textureA, Texture* textureB, std::string shader);
	virtual ~RadioButton();

	virtual void Update(float delta);
	virtual void Render();

	virtual void CallBack();
	void SetOtherCallBack(std::function<void()> callback);

	void SetOn(bool on) { _IsOn = on; }

private:
	Sprite * _ButtonOnSprite;

	bool _IsOn;

	std::function<void()> _OtherCallBack;
};

