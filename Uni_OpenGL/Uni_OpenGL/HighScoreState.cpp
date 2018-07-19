#include "HighScoreState.h"

#include "StateManager.h"
#include "InputManager.h"
#include "ScreenManager.h"
#include "ResourceManager.h"

#include "FileLoader.h"
#include "Tools.h"

HighScoreState::HighScoreState()
{
}

HighScoreState::~HighScoreState()
{
}

bool HighScoreState::Initialize()
{
	Texture* buttonBack = ResourceManager::Instance()->GetTexture("ButtonBase");
	_TextRenderer = ResourceManager::Instance()->GetTextRenderer("Font_Calibri");
	glm::vec2 screenSize = ScreenManager::Instance()->GetSize();

	//~~~~~Back Button~~~~~/
	Button* button = new Button(glm::vec2(screenSize.x / 20, screenSize.y / 10 - 50), glm::vec2(300, 50), buttonBack, "texture");
	button->SetButtonText("Back");
	button->SetCallBack([this]() {
		StateManager::Instance()->PopState();
	});
	_MenuButtons.push_back(button);
	//~~~~~Left Button~~~~~/
	button = new Button(glm::vec2(screenSize.x / 20, screenSize.y / 10), glm::vec2(50, 50), ResourceManager::Instance()->GetTexture("ButtonLeft"), "texture");
	button->SetButtonText("");
	button->SetCallBack([this]() { _SelectedSeed--; CreatePreview(); });
	_MenuButtons.push_back(button);
	//~~~~~Right Button~~~~~/
	button = new Button(glm::vec2(screenSize.x / 20 + 250, screenSize.y / 10), glm::vec2(50, 50), ResourceManager::Instance()->GetTexture("ButtonRight"), "texture");
	button->SetButtonText("");
	button->SetCallBack([this]() { _SelectedSeed++; CreatePreview(); });
	_MenuButtons.push_back(button);

	_SceneSky = ResourceManager::Instance()->GetSkyBox("DesertSkyBox");
	_Camera = ResourceManager::Instance()->GetCamera();
	_HighScoreList = ResourceManager::Instance()->GetHighScores();

	//~~~~~~Create a track spline for preview~~~~~~//
	_PreviewTrack = new TrackGenerator();
	_PreviewSpline = new CatmullRomSpline();

	CreatePreview();

	ScreenManager::Instance()->ReleaseMouse();
	InputManager::Instance()->ReleaseMouse();

	return true;
}

void HighScoreState::Input()
{
	if (InputManager::Instance()->IsButtonPressed(SDL_BUTTON_LEFT)) {
		for (auto b : _MenuButtons) {
			if (b->IsColliding(glm::vec2(InputManager::Instance()->GetMouseMove().xPos, InputManager::Instance()->GetMouseMove().yPos))) {
				b->CallBack();
				InputManager::Instance()->ResetMouseEvents();
			}
		}
	}
	if (InputManager::Instance()->IsKeyPressed(SDLK_ESCAPE)) {
		StateManager::Instance()->PopState();
	}
}

void HighScoreState::Update(float delta)
{
	_SkyboxSpinAngle += 0.001f;
}

void HighScoreState::Render()
{
	//~~~~~~~ALL 3D RENDERING~~~~~~//
	ScreenManager::Instance()->Set3D(90.0f, _Camera->GetZoom(), 0.1f, 1000.0f);
	glm::vec2 screenSize = ScreenManager::Instance()->GetSize();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, _SkyboxSpinAngle, glm::vec3(0, 1, 0));
	ShaderManager::Instance()->GetShader("skybox")->UpdateMatrices(model, _Camera->GetViewMatrix(), ScreenManager::Instance()->GetProjection());
	_SceneSky->Render();

	_PreviewSpline->Render();

	//~~~~~~~ALL 2D RENDERING~~~~~~//
	ScreenManager::Instance()->Set2D();

	model = glm::mat4(1.0f);
	ShaderManager::Instance()->GetShader("texture")->UpdateMatrices(model, _Camera->GetOrthoView(), ScreenManager::Instance()->GetProjection());
	for (auto b : _MenuButtons) {
		b->Render();
		ShaderManager::Instance()->GetShader("texture")->UpdateMatrices(model, _Camera->GetOrthoView(), ScreenManager::Instance()->GetProjection());
		glm::vec2 pos = b->GetPosition();
		_TextRenderer->RenderText(b->GetButtonText(), glm::vec2(pos.x + 6, pos.y + 10), 1);
	}
	_TextRenderer->RenderText("High Scores", glm::vec2(screenSize.x * 0.5f, screenSize.y * 0.85), 2, true);

	HighScore _Score = GetHighScore(_SelectedSeed);
	_TextRenderer->RenderText("Level Seed: " + std::to_string(_SelectedSeed), glm::vec2(screenSize.x * 0.75f, screenSize.y * 0.5f), 0.7f, true, glm::vec3(0, 0, 0));
	
	_TextRenderer->RenderText("Lap Time: " + FloatToTime(_Score._LapTime), glm::vec2(screenSize.x * 0.75f, (screenSize.y * 0.5f) - 50), 0.7f, true, glm::vec3(0, 0, 0));
	_TextRenderer->RenderText("Initials: " + _Score._Initials, glm::vec2(screenSize.x * 0.75f, (screenSize.y * 0.5f) + 50), 0.7f, true, glm::vec3(0, 0, 0));

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3((screenSize.x / 7.5f) + 20, screenSize.y * 0.5f, 0.0f));
	ShaderManager::Instance()->GetShader("basic")->UpdateMatrices(model, _Camera->GetOrthoView(), ScreenManager::Instance()->GetProjection());
	_PreviewSpline->Render();
}

bool HighScoreState::Shutdown()
{
	for (auto b : _MenuButtons) {
		delete b;
	}
	_MenuButtons.clear();

	delete _PreviewTrack;
	delete _PreviewSpline;

	return true;
}

void HighScoreState::Pause()
{
	_IsPaused = true;
}

void HighScoreState::Resume()
{
	_IsPaused = false;
}

bool HighScoreState::IsPaused()
{
	return _IsPaused;
}

void HighScoreState::CreatePreview()
{
	_PreviewTrack->SetSeed(_SelectedSeed);
	_PreviewTrack->GeneratePoints();

	_PreviewSpline->SetOriginalPoints(*_PreviewTrack->GetPoints());
	_PreviewSpline->SetCurvePointCount(50.0f);
	_PreviewSpline->GenerateSpline();
	_PreviewSpline->AddToBuffer2D();
}

HighScore HighScoreState::GetHighScore(int seed)
{
	auto search = _HighScoreList->find(seed);
	if (search != _HighScoreList->end()) {
		return _HighScoreList->at(seed);
	}
	else {
		HighScore _Score;
		_Score._LapTime = 0.0f;
		_Score._Initials = "---";
		return _Score;
	}
}
