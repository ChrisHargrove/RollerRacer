#pragma once

#include "State.h"
#include "Button.h"
#include "Camera.h"
#include "SkyBox.h"
#include "TextRenderer.h"
#include "Structures.h"

#include <vector>

class AddScoreState : public State
{
public:
	AddScoreState();
	virtual ~AddScoreState();

	// Inherited via State
	virtual bool Initialize() override;
	virtual void Input() override;
	virtual void Update(float delta) override;
	virtual void Render() override;
	virtual bool Shutdown() override;
	virtual void Pause() override;
	virtual void Resume() override;
	virtual bool IsPaused() override;

	void PassLapTime(float laptime);
	void PassSeed(int seed);
private:
	std::vector<Button*> _MenuButtons;
	SkyBox* _SceneSky;
	Camera* _Camera;

	int _Initials[3];
	float _LapTime = 0.0f;
	int _Seed;

	Button* _FirstBtn;
	Button* _SecondBtn;
	Button* _ThirdBtn;

	float _SkyboxSpinAngle = 0.0;
	TextRenderer* _TextRenderer;

	std::map<int, HighScore>* _HighScoreList;

	void AddHighScore();

};

