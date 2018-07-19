#pragma once
#include "State.h"
#include "Button.h"
#include "Camera.h"
#include "SkyBox.h"
#include "TextRenderer.h"

#include <vector>

class MenuState : public State
{
public:
	MenuState();
	virtual ~MenuState();

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
	SkyBox* _SceneSky;
	Camera* _Camera;

	float _SkyboxSpinAngle = 0.0;
	TextRenderer* _TextRenderer;
};