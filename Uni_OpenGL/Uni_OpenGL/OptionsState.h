#pragma once

#include "State.h"
#include "Button.h"
#include "RadioButton.h"
#include "Camera.h"
#include "SkyBox.h"
#include "TextRenderer.h"
#include <vector>
#include <map>

class OptionsState : public State
{
public:
	OptionsState();
	virtual ~OptionsState();

	// Inherited via State
	virtual bool Initialize() override;
	virtual void Input() override;
	virtual void Update(float delta) override;
	virtual void Render() override;
	virtual bool Shutdown() override;
	virtual void Pause() override;
	virtual void Resume() override;
	virtual bool IsPaused() override;

private:
	std::vector<Button*> _MenuButtons;
	std::vector<RadioButton*> _RadioButtons;
	SkyBox* _SceneSky;
	Camera* _Camera;

	float _SkyboxSpinAngle = 0.0;
	TextRenderer* _TextRenderer;

	std::map<std::string, int>* _OptionsData;

	void TurnOnToonShading(bool turnOn);
};

