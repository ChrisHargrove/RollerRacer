#pragma once

#include "State.h"
#include "Button.h"
#include "Camera.h"
#include "SkyBox.h"
#include "TextRenderer.h"
#include "Sprite.h"

#include "TrackGenerator.h"
#include "CatmullRomSpline.h"

#include <vector> 

class LevelSelectState : public State
{
public:
	LevelSelectState();
	virtual ~LevelSelectState();

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
	Sprite* _TrackBackSquare;

	int _SelectedSeed = 0;
	TrackGenerator* _PreviewTrack;
	CatmullRomSpline* _PreviewSpline;

	float _SkyboxSpinAngle = 0.0;
	TextRenderer* _TextRenderer;

	void CreatePreview();
};