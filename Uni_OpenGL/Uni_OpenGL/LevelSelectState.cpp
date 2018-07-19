#include "LevelSelectState.h"
#include "Texture.h"
#include "StateManager.h"
#include "ScreenManager.h"
#include "InputManager.h"
#include "PlayState.h"
#include "ResourceManager.h"


LevelSelectState::LevelSelectState()
{
}

LevelSelectState::~LevelSelectState()
{
}

bool LevelSelectState::Initialize()
{
	Texture* buttonBack = ResourceManager::Instance()->GetTexture("ButtonBase");
	_TextRenderer = ResourceManager::Instance()->GetTextRenderer("Font_Calibri");

	glm::vec2 screenSize = ScreenManager::Instance()->GetSize();
	//~~~~~Play Button~~~~~/
	Button* button = new Button(glm::vec2(screenSize.x /20 , screenSize.y / 10), glm::vec2(300, 50), buttonBack, "texture");
	button->SetButtonText("Play");
	button->SetCallBack([this]() {
		((PlayState*)StateManager::Instance()->GetState("[STATE]Play"))->SetSeed(_SelectedSeed);
		StateManager::Instance()->PushState("[STATE]Play");
	});
	_MenuButtons.push_back(button);
	//~~~~~Back Button~~~~~/
	button = new Button(glm::vec2(screenSize.x - screenSize.x / 20 - 300, screenSize.y / 10), glm::vec2(300, 50), buttonBack, "texture");
	button->SetButtonText("Back");
	button->SetCallBack([]() { StateManager::Instance()->PopState(); });
	_MenuButtons.push_back(button);
	//~~~~~Left Button~~~~~/
	button = new Button(glm::vec2(screenSize.x / 10, (screenSize.y * 0.5f) - 25.0f), glm::vec2(50, 50), ResourceManager::Instance()->GetTexture("ButtonLeft"), "texture");
	button->SetButtonText("");
	button->SetCallBack([this]() { _SelectedSeed--; CreatePreview(); });
	_MenuButtons.push_back(button);
	//~~~~~Right Button~~~~~/
	button = new Button(glm::vec2(screenSize.x - screenSize.x / 10 - 50, (screenSize.y * 0.5f) - 25.0f), glm::vec2(50, 50), ResourceManager::Instance()->GetTexture("ButtonRight"), "texture");
	button->SetButtonText("");
	button->SetCallBack([this]() { _SelectedSeed++; CreatePreview(); });
	_MenuButtons.push_back(button);

	_SceneSky = ResourceManager::Instance()->GetSkyBox("DesertSkyBox");
	_Camera = ResourceManager::Instance()->GetCamera();

	//~~~~~~Create a track spline for preview~~~~~~//
	_PreviewTrack = new TrackGenerator();
	_PreviewSpline = new CatmullRomSpline();

	CreatePreview();

	ScreenManager::Instance()->ReleaseMouse();
	InputManager::Instance()->ReleaseMouse();
	return true;
}

void LevelSelectState::Input()
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

void LevelSelectState::Update(float delta)
{
	_SkyboxSpinAngle += 0.001f;
}

void LevelSelectState::Render()
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
	_TextRenderer->RenderText("Level Select", glm::vec2(screenSize.x * 0.5f, screenSize.y * 0.85f), 2, true);

	model = glm::mat4(1.0f);
	glm::vec2 screenCenter = ScreenManager::Instance()->GetSize() * 0.5f;
	model = glm::translate(model, glm::vec3(screenCenter, 0.0f));
	ShaderManager::Instance()->GetShader("basic")->UpdateMatrices(model, _Camera->GetOrthoView(), ScreenManager::Instance()->GetProjection());
	_PreviewSpline->Render();
	
}

bool LevelSelectState::Shutdown()
{
	for (auto b : _MenuButtons) {
		delete b;
	}
	_MenuButtons.clear();

	delete _PreviewTrack;
	delete _PreviewSpline;

	return true;
}

void LevelSelectState::Pause()
{
	_IsPaused = true;
}

void LevelSelectState::Resume()
{
	_IsPaused = false;
	ScreenManager::Instance()->ReleaseMouse();
	InputManager::Instance()->ReleaseMouse();
}

bool LevelSelectState::IsPaused()
{
	return _IsPaused;
}

void LevelSelectState::CreatePreview()
{
	_PreviewTrack->SetSeed(_SelectedSeed);
	_PreviewTrack->GeneratePoints();

	_PreviewSpline->SetOriginalPoints(*_PreviewTrack->GetPoints());
	_PreviewSpline->SetCurvePointCount(50.0f);
	_PreviewSpline->GenerateSpline();
	_PreviewSpline->AddToBuffer2D();
}
