#pragma once

#include "State.h"

#include "Grid.h"
#include "Axis.h"
#include "SkyBox.h"
#include "Camera.h"
#include "Light.h"

#include "Speedometer.h"
#include "TextRenderer.h"
#include "Structures.h"

#include "Model.h"
#include "Terrain.h"
#include "Sprite.h"

#include <BULLET/btBulletCollisionCommon.h>
#include <Bullet/btBulletDynamicsCommon.h>
#include "BGameObject.h"
#include "BVehicle.h"

#include "Level.h"

#include "TrackGenerator.h"
#include "CatmullRomSpline.h"

#include "ShadowMapBuffer.h"

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

	void SetSeed(int seed);

private:
    Grid* _SceneGrid;
    Axis* _SceneAxis;
    SkyBox* _SceneSky;
    Camera* _Camera;

	DirectionalLight* _DirectionalLight;
	PointLight*		_PointLight;

	BVehicle* _Car;

	bool EnableWireMode = false;

	std::vector<BGameObject*> _PhysicsObjects;

	Speedometer* _CarSpeedometer;
	TextRenderer* _TextRenderer;

	Level* _Level;

	int _Seed = 0;

	std::map<int, HighScore>* _HighScoreList;

	bool CompareHighScore(float lapTime);

	ShadowMapBuffer _ShadowBuffer;
	Sprite* DepthSprite;

};

