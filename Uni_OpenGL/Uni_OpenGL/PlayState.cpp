#include "PlayState.h"

#include "ScreenManager.h"
#include "StateManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "ResourceManager.h"
#include "Texture.h"

#include "BBox.h"
#include "BModel.h"
#include "BVehicle.h"
#include "BTriggerVolume.h"

#include "MathUtil.h"
#include "AddScoreState.h"

#include "Tools.h"



PlayState::PlayState()
{
}


PlayState::~PlayState()
{
}

bool PlayState::Initialize()
{
    _SceneGrid = new Grid(7, "basic");
    _SceneAxis = new Axis(7, "basic");

	_SceneSky = ResourceManager::Instance()->GetSkyBox("DesertSkyBox");

	_Camera = ResourceManager::Instance()->GetCamera();

	_DirectionalLight = new DirectionalLight(glm::vec3(0.0f, -1.0f, 0.4f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(0.8f, 0.8f, 0.8f));
	_PointLight = new PointLight(glm::vec3(0.0f, 5.0f, 3.0f), 1.0f,	0.045f,	0.0075f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(1.0f, 1.0f, 1.0f));

    std::vector<Texture*> textureList;
    textureList.push_back(ResourceManager::Instance()->GetTexture("TerrainSand"));
    textureList.push_back(ResourceManager::Instance()->GetTexture("TerrainRoad"));

	std::vector<Model*> foliageList;
	foliageList.push_back(ResourceManager::Instance()->GetModel("CactiOne"));
	foliageList.push_back(ResourceManager::Instance()->GetModel("CactiTwo"));

	glm::vec2 screenSize = ScreenManager::Instance()->GetSize();
	_CarSpeedometer = new Speedometer(glm::vec2(200,200), "texture");
	_CarSpeedometer->SetPosition(glm::vec2((screenSize.x - _CarSpeedometer->GetSize().x), 0));
	
	PhysicsManager::Instance()->Initialize();

	_Level = new Level(_Seed, textureList, foliageList);
	_Level->CreateLevel();
	PhysicsManager::Instance()->AddLevelToSimulation(_Level);

    Model* CarModel = ResourceManager::Instance()->GetModel("Buggy");
	_Car = new BVehicle(_Level->GetNextTriggerGate()->GetPosition() + glm::vec3(0,3,0), glm::vec3(0, 1, 0), 739.8092f, CarModel, "betterLight");
	_Car->Scale(glm::vec3(0.5));

    _Camera->SetTarget(_Car);
    _PhysicsObjects.push_back(_Car);
    for (auto p : _PhysicsObjects) {
        PhysicsManager::Instance()->AddObjectToSimulation(p);
    }
	_Car->SetDirection(_Level->GetNextTriggerGate()->GetTrackDirection());
	_CarSpeedometer->SetTarget(_Car);

	_TextRenderer = ResourceManager::Instance()->GetTextRenderer("Font_Calibri");

	_HighScoreList = ResourceManager::Instance()->GetHighScores();

	ScreenManager::Instance()->GrabMouse();
	InputManager::Instance()->GrabMouse();

    return true;
}

void PlayState::Input()
{
	PhysicsManager::Instance()->Input();

	if (InputManager::Instance()->IsKeyPressed(SDLK_ESCAPE)) {
		StateManager::Instance()->PushState("[STATE]Pause");
		if (CompareHighScore(_Level->GetBestLapTime())) {
			((AddScoreState*)StateManager::Instance()->GetState("[STATE]AddScore"))->PassLapTime(_Level->GetBestLapTime());
			((AddScoreState*)StateManager::Instance()->GetState("[STATE]AddScore"))->PassSeed(_Seed);
			StateManager::Instance()->PushState("[STATE]AddScore");
		}
	}

	if (InputManager::Instance()->IsKeyPressed(SDLK_F8)) {
		if (EnableWireMode) {
			EnableWireMode = !EnableWireMode;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			EnableWireMode = !EnableWireMode;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	if (InputManager::Instance()->IsKeyPressed(SDLK_BACKSPACE)) {
		int index = _Level->GetNearestTrackPoint(_Car->GetPosition());

		glm::vec2 point = _Level->GetSmoothTrack()->GetSpline()[index];
		glm::vec2 next;
		//wrap to front if at end of spline
		if (index < (int)_Level->GetSmoothTrack()->GetSpline().size() - 1) {
			 next = _Level->GetSmoothTrack()->GetSpline()[index + 1];
		}
		else {
			next = _Level->GetSmoothTrack()->GetSpline()[0];
		}
		glm::vec2 terrainPoint = point / _Level->GetTerrain()->GetVertexSpacing();

		_Car->SetLinearVelocity(glm::vec3(0, 0, 0));
		_Car->SetAngularVelocity(glm::vec3(0, 0, 0));

		_Car->SetPosition(glm::vec3(point.x, 2.0f, point.y));
		_Car->SetDirection(glm::vec3(next.x, 2.0f, next.y) - glm::vec3(point.x, 2.0f, point.y));
	}

	
    for (auto p : _PhysicsObjects) {
        p->Input();
    }
}

void PlayState::Update(float delta)
{
	PhysicsManager::Instance()->Update(delta);

    for (auto p : _PhysicsObjects) {
        p->Update(delta);
    }
	
	_Camera->Update();
	_CarSpeedometer->Update(delta);
	_Level->Update(delta);

	if (PhysicsManager::Instance()->CheckSingleCollision(_Level->GetNextTriggerGate(), _Car)) {
		_Level->GetNextTriggerGate()->CallBack();
	}

}

void PlayState::Render()
{
	//~~~~~~~ALL 3D RENDERING~~~~~~//
	ScreenManager::Instance()->Set3D(90, _Camera->GetZoom(), 0.1f, 1000.0f);
		
	glm::mat4 model = glm::mat4(1.0f);
	
    _DirectionalLight->SendToShader("betterLight");
    _PointLight->SendToShader(0, "betterLight");
    ShaderManager::Instance()->GetShader("betterLight")->SetVec3("viewPos", _Camera->GetPosition());
    for (auto p : _PhysicsObjects) {
        ShaderManager::Instance()->GetShader(p->GetShader())->UpdateMatrices(p->GetModelMatrix(), _Camera->GetViewMatrix(), ScreenManager::Instance()->GetProjection());
        p->Render();
    }

    model = glm::mat4(1.0f);
	model = glm::translate(glm::vec3(-400, 0, -400));
    ShaderManager::Instance()->GetShader("terrain")->UpdateMatrices(model, _Camera->GetViewMatrix(), ScreenManager::Instance()->GetProjection());
    _DirectionalLight->SendToShader("terrain");
    _PointLight->SendToShader(0, "terrain");
    ShaderManager::Instance()->GetShader("terrain")->SetVec3("viewPos", _Camera->GetPosition());
	_Level->Render();

    model = glm::mat4(1.0f);
    ShaderManager::Instance()->GetShader("skybox")->UpdateMatrices(model, _Camera->GetViewMatrix(), ScreenManager::Instance()->GetProjection());
    _SceneSky->Render();

    ShaderManager::Instance()->GetShader("debug")->UpdateMatrices(model, _Camera->GetViewMatrix(), ScreenManager::Instance()->GetProjection());
	PhysicsManager::Instance()->Render();

    //~~~~~~~ALL 2D RENDERING~~~~~~//
    ScreenManager::Instance()->Set2D();

	model = glm::mat4(1.0f);
	ShaderManager::Instance()->GetShader("texture")->UpdateMatrices(model, _Camera->GetOrthoView(), ScreenManager::Instance()->GetProjection());
	_CarSpeedometer->Render();
	model = glm::mat4(1.0f);
	ShaderManager::Instance()->GetShader("texture")->UpdateMatrices(model, _Camera->GetOrthoView(), ScreenManager::Instance()->GetProjection());
	//lap time needs formatting to be in minutes and seconds rather than just seconds and milliseconds.
	glm::vec2 screenSize = ScreenManager::Instance()->GetSize();
	_TextRenderer->RenderText("Current Lap Time", glm::vec2(screenSize.x / 20, screenSize.y - 50), 1);
	_TextRenderer->RenderText(FloatToTime(_Level->GetCurrentLapTime()), glm::vec2(screenSize.x / 20, screenSize.y - 100), 1);

}

bool PlayState::Shutdown()
{
    delete _SceneAxis;
    delete _SceneGrid;
	delete _DirectionalLight;
	delete _PointLight;;
	delete _Car;
	delete _CarSpeedometer;
	delete _Level;

	_PhysicsObjects.clear();
	
	PhysicsManager::Instance()->Shutdown();

    return true;
}

void PlayState::Pause()
{
    _IsPaused = true;
	_Level->GetStopWatch()->Pause();
}

void PlayState::Resume()
{
    _IsPaused = false;
	_Level->GetStopWatch()->Resume();
	ScreenManager::Instance()->GrabMouse();
	InputManager::Instance()->GrabMouse();
}

bool PlayState::IsPaused()
{
    return _IsPaused;
}

void PlayState::SetSeed(int seed)
{
	_Seed = seed;
}

bool PlayState::CompareHighScore(float lapTime)
{
	if (lapTime == 0.0f) {
		return false;
	}
	auto search = _HighScoreList->find(_Seed);
	if (search != _HighScoreList->end()) {
		if (_HighScoreList->at(_Seed)._LapTime > lapTime) {
			return true;
		}
	}
	else {
		return true;
	}
	return false;
}
