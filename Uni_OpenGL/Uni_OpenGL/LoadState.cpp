#include "LoadState.h"
#include "ShaderManager.h"
#include "StateManager.h"
#include "ResourceManager.h"

#include "MenuState.h"
#include "PlayState.h"
#include "PauseState.h"
#include "LevelSelectState.h"
#include "InstructionsState.h"
#include "HighScoreState.h"
#include "AddScoreState.h"
#include "OptionsState.h"

#include <map>

LoadState::LoadState() :
    _HasFinishedLoading(false)
{
    _IsPaused = false;
}


LoadState::~LoadState()
{
}

bool LoadState::Initialize()
{
    ShaderManager::Instance()->AddShader("basic", "basic");
    ShaderManager::Instance()->AddShader("debug", "debug");
    ShaderManager::Instance()->AddShader("skybox", "skybox");
    ShaderManager::Instance()->AddShader("betterLight", "betterLight");
	ShaderManager::Instance()->AddShader("texture", "texture");
    ShaderManager::Instance()->AddShader("terrain", "terrain");
	ShaderManager::Instance()->AddShader("shadow", "shadow");

	if (!StateManager::Instance()->AddState("[STATE]Menu", new MenuState())) {
		return false;
	}
    if (!StateManager::Instance()->AddState("[STATE]Play", new PlayState())) {
        return false;
    }
	if (!StateManager::Instance()->AddState("[STATE]Pause", new PauseState())) {
		return false;
	}
	if (!StateManager::Instance()->AddState("[STATE]Level", new LevelSelectState())) {
		return false;
	}
	if (!StateManager::Instance()->AddState("[STATE]Instructions", new InstructionsState())) {
		return false;
	}
	if (!StateManager::Instance()->AddState("[STATE]HighScores", new HighScoreState())) {
		return false;
	}
	if (!StateManager::Instance()->AddState("[STATE]AddScore", new AddScoreState())) {
		return false;
	}
	if (!StateManager::Instance()->AddState("[STATE]Options", new OptionsState())) {
		return false;
	}
	//~~~~~~Load Highscores~~~~~~~~//
	ResourceManager::Instance()->LoadHighScores("Data/highscores.dat");
	ResourceManager::Instance()->LoadOptions("Data/options.dat");

	//~~~~~~Load All Textures~~~~~~//
	ResourceManager::Instance()->LoadTexture("Assets/Textures/btnBase.png", "ButtonBase");
	ResourceManager::Instance()->LoadTexture("Assets/Textures/btnRadio.png", "ButtonRadio");
	ResourceManager::Instance()->LoadTexture("Assets/Textures/btnLeft.png", "ButtonLeft");
	ResourceManager::Instance()->LoadTexture("Assets/Textures/btnRight.png", "ButtonRight");
	ResourceManager::Instance()->LoadTexture("Assets/Textures/btnUp.png", "ButtonUp");
	ResourceManager::Instance()->LoadTexture("Assets/Textures/btnDown.png", "ButtonDown");
	ResourceManager::Instance()->LoadTexture("Assets/Textures/Terrain/sand.jpg", "TerrainSand");
	ResourceManager::Instance()->LoadTexture("Assets/Textures/Terrain/sandRoad.jpg", "TerrainRoad");
	ResourceManager::Instance()->LoadTexture("Assets/Textures/Speedometer/SpeedoDial.png", "SpeedoDial");
	ResourceManager::Instance()->LoadTexture("Assets/Textures/Speedometer/SpeedoNeedle.png", "SpeedoNeedle");

	//~~~~~~Load All Skybox's~~~~~~//
	ResourceManager::Instance()->LoadSkybox("Assets/Textures/Skybox/hw_sahara/D", "DesertSkyBox");

	//~~~~~~Load All Text Renderers~~~~~~//
	ResourceManager::Instance()->LoadTextRenderer("Assets/Fonts/calibri.ttf", "Font_Calibri");

	//~~~~~~Create Game Camera~~~~~~//
	ResourceManager::Instance()->LoadCamera(glm::vec3(0,0,0));

	//~~~~~~Load All Models~~~~~~//
	ResourceManager::Instance()->LoadModel("Assets/Models/Cacti/cactus_one.obj", "betterLight", "CactiOne");
	ResourceManager::Instance()->LoadModel("Assets/Models/Cacti/cactus_two.obj", "betterLight", "CactiTwo");
	ResourceManager::Instance()->LoadModel("Assets/Models/Buggy/Buggy.obj", "betterLight", "Buggy");

    return true;
}

void LoadState::Input()
{
}

void LoadState::Update(float delta)
{
	std::map<std::string, int>* options = ResourceManager::Instance()->GetOptions();
	if (options->find("ToonShading") != options->end()) {
		if (options->at("ToonShading") == 1) {
			ShaderManager::Instance()->GetShader("terrain")->SetBool("toonShadingOn", true);
			ShaderManager::Instance()->GetShader("betterLight")->SetBool("toonShadingOn", true);
		}
	}

	_HasFinishedLoading = true;
	if (_HasFinishedLoading) {
		if (!StateManager::Instance()->PushState("[STATE]Menu")) {
		}
	}
}

void LoadState::Render()
{
}

bool LoadState::Shutdown()
{
	ResourceManager::Instance()->SaveHighScores("Data/highscores.dat");
	ResourceManager::Instance()->UnloadAllResources();
    return true;
}

void LoadState::Pause()
{
    _IsPaused = true;
}

void LoadState::Resume()
{
    _IsPaused = false;
}

bool LoadState::IsPaused()
{
    return _IsPaused;
}
