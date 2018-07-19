#include "AddScoreState.h"
#include "Texture.h"
#include "StateManager.h"
#include "ScreenManager.h"
#include "InputManager.h"
#include "PlayState.h"
#include "ResourceManager.h"
#include "Tools.h"


AddScoreState::AddScoreState()
{
	
}

AddScoreState::~AddScoreState()
{
}

bool AddScoreState::Initialize()
{
	for (int i = 0; i < 3; i++) {
		_Initials[i] = 'A';
	}

	Texture* buttonBack = ResourceManager::Instance()->GetTexture("ButtonBase");
	Texture* buttonUp = ResourceManager::Instance()->GetTexture("ButtonUp");
	Texture* buttonDown = ResourceManager::Instance()->GetTexture("ButtonDown");
	_TextRenderer = ResourceManager::Instance()->GetTextRenderer("Font_Calibri");
	glm::vec2 screenSize = ScreenManager::Instance()->GetSize();

	//~~~~~Submit Button~~~~~/
	Button* button = new Button(glm::vec2((screenSize.x * 0.2f) - 150, screenSize.y * 0.1f), glm::vec2(300, 50), buttonBack, "texture");
	button->SetButtonText("Submit");
	button->SetCallBack([this]() { AddHighScore(); StateManager::Instance()->PopState(); });
	_MenuButtons.push_back(button);
	//~~~~~1st Up Button~~~~~/
	button = new Button(glm::vec2((screenSize.x * 0.5f) - 85, (screenSize.y * 0.5f) + 85), glm::vec2(50, 50), buttonUp, "texture");
	button->SetButtonText("");
	button->SetCallBack([this]() { _Initials[0]++; _FirstBtn->SetButtonText((char)_Initials[0]); });
	_MenuButtons.push_back(button);
	//~~~~~2nd Up Button~~~~~/
	button = new Button(glm::vec2((screenSize.x * 0.5f) - 25, (screenSize.y * 0.5f) + 85), glm::vec2(50, 50), buttonUp, "texture");
	button->SetButtonText("");
	button->SetCallBack([this]() {_Initials[1]++; _SecondBtn->SetButtonText((char)_Initials[1]); });
	_MenuButtons.push_back(button);
	//~~~~~3rd Up Button~~~~~/
	button = new Button(glm::vec2((screenSize.x * 0.5f) + 85, (screenSize.y * 0.5f) + 85), glm::vec2(50, 50), buttonUp, "texture");
	button->SetButtonText("");
	button->SetCallBack([this]() {_Initials[2]++; _ThirdBtn->SetButtonText((char)_Initials[2]); });
	_MenuButtons.push_back(button);

	//~~~~~1st Down Button~~~~~/
	button = new Button(glm::vec2((screenSize.x * 0.5f) - 85, (screenSize.y * 0.5f) - 85), glm::vec2(50, 50) , buttonDown, "texture");
	button->SetButtonText("");
	button->SetCallBack([this]() { _Initials[0]--; _FirstBtn->SetButtonText((char)_Initials[0]); });
	_MenuButtons.push_back(button);
	//~~~~~2nd Down Button~~~~~/
	button = new Button(glm::vec2((screenSize.x * 0.5f) - 25, (screenSize.y * 0.5f) - 85), glm::vec2(50, 50), buttonDown, "texture");
	button->SetButtonText("");
	button->SetCallBack([this]() { _Initials[1]--; _SecondBtn->SetButtonText((char)_Initials[1]); });
	_MenuButtons.push_back(button);
	//~~~~~3rd Down Button~~~~~/
	button = new Button(glm::vec2((screenSize.x * 0.5f) + 85, (screenSize.y * 0.5f) - 85), glm::vec2(50, 50), buttonDown, "texture");
	button->SetButtonText("");
	button->SetCallBack([this]() { _Initials[2]--; _ThirdBtn->SetButtonText((char)_Initials[2]); });
	_MenuButtons.push_back(button);

	//~~~~~1st Non Button~~~~~/
	_FirstBtn = new Button(glm::vec2((screenSize.x * 0.5f) - 85, (screenSize.y * 0.5f) + 25), glm::vec2(50, 50), buttonBack, "texture");
	_FirstBtn->SetButtonText((char)_Initials[0]);
	_FirstBtn->SetCallBack([]() {});
	_MenuButtons.push_back(_FirstBtn);
	//~~~~~2nd Non Button~~~~~/
	_SecondBtn = new Button(glm::vec2((screenSize.x * 0.5f) - 25, (screenSize.y * 0.5f) + 25), glm::vec2(50, 50), buttonBack, "texture");
	_SecondBtn->SetButtonText((char)_Initials[1]);
	_SecondBtn->SetCallBack([]() {});
	_MenuButtons.push_back(_SecondBtn);
	//~~~~~3rd Non Button~~~~~/
	_ThirdBtn = new Button(glm::vec2((screenSize.x * 0.5f) + 85, (screenSize.y * 0.5f) +25), glm::vec2(50, 50), buttonBack, "texture");
	_ThirdBtn->SetButtonText((char)_Initials[2]);
	_ThirdBtn->SetCallBack([]() {});
	_MenuButtons.push_back(_ThirdBtn);

	_HighScoreList = ResourceManager::Instance()->GetHighScores();

	_SceneSky = ResourceManager::Instance()->GetSkyBox("DesertSkyBox");
	_Camera = ResourceManager::Instance()->GetCamera();
	ScreenManager::Instance()->ReleaseMouse();
	InputManager::Instance()->ReleaseMouse();

	return true;
}

void AddScoreState::Input()
{
	if (InputManager::Instance()->IsButtonPressed(SDL_BUTTON_LEFT)) {
		for (auto b : _MenuButtons) {
			if (b->IsColliding(glm::vec2(InputManager::Instance()->GetMouseMove().xPos, InputManager::Instance()->GetMouseMove().yPos))) {
				b->CallBack();
				InputManager::Instance()->ResetMouseEvents();
			}
		}
	}
}

void AddScoreState::Update(float delta)
{
	_SkyboxSpinAngle += 0.001f;
}

void AddScoreState::Render()
{
	//~~~~~~~ALL 3D RENDERING~~~~~~//
	ScreenManager::Instance()->Set3D(90.0f, _Camera->GetZoom(), 0.1f, 1000.0f);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, _SkyboxSpinAngle, glm::vec3(0, 1, 0));
	ShaderManager::Instance()->GetShader("skybox")->UpdateMatrices(model, _Camera->GetViewMatrix(), ScreenManager::Instance()->GetProjection());
	_SceneSky->Render();

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
	glm::vec2 screenSize = ScreenManager::Instance()->GetSize();
	_TextRenderer->RenderText("New High Score", glm::vec2(screenSize.x * 0.5f, screenSize.y * 0.9), 2, true);
	_TextRenderer->RenderText(FloatToTime(_LapTime), glm::vec2(screenSize.x * 0.5f, screenSize.y * 0.7), 1.5f, true);
	
}

bool AddScoreState::Shutdown()
{
	for (auto b : _MenuButtons) {
		delete b;
	}
	_MenuButtons.clear();
	return true;
}

void AddScoreState::Pause()
{
	_IsPaused = true;
}

void AddScoreState::Resume()
{
	_IsPaused = false;
}

bool AddScoreState::IsPaused()
{
	return _IsPaused;
}

void AddScoreState::PassLapTime(float laptime)
{
	_LapTime = laptime;
}

void AddScoreState::PassSeed(int seed)
{
	_Seed = seed;
}

void AddScoreState::AddHighScore()
{
	auto search = _HighScoreList->find(_Seed);
	if (search != _HighScoreList->end()) {
		_HighScoreList->at(_Seed)._LapTime = _LapTime;
		std::string temp = "";
		temp.push_back((char)_Initials[0]);
		temp.push_back((char)_Initials[1]);
		temp.push_back((char)_Initials[2]);
		_HighScoreList->at(_Seed)._Initials = temp;
	}
	else {
		HighScore score;
		std::string temp = "";
		temp.push_back((char)_Initials[0]);
		temp.push_back((char)_Initials[1]);
		temp.push_back((char)_Initials[2]);
		score._Initials = temp;
		score._LapTime = _LapTime;
		_HighScoreList->emplace(std::make_pair(_Seed, score));
	}
}
