#pragma once

#include "State.h"

#include "Grid.h"
#include "Axis.h"
#include "SkyBox.h"
#include "Camera.h"
#include "Light.h"

#include "Model.h"

class PlayState : public State
{
public:
    PlayState();
    virtual ~PlayState();

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
    Grid* _SceneGrid;
    Axis* _SceneAxis;
    SkyBox* _SceneSky;
    Camera* _Camera;

	DirectionalLight* _DirectionalLight;
	PointLight*		_PointLight;

	Model* test;
};

