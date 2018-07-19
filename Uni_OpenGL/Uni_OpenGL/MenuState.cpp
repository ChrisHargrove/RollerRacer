#include "MenuState.h"
#include "Texture.h"
#include "StateManager.h"
#include "ScreenManager.h"
#include "InputManager.h"
#include "PlayState.h"
#include "ResourceManager.h"

MenuState::MenuState()
{
}

MenuState::~MenuState()
{
}

bool MenuState::Initialize()
{
	Texture* buttonBack = ResourceManager::Instance()->GetTexture("ButtonBase");
	_TextRenderer = ResourceManager::Instance()->GetTextRenderer("Font_Calibri");

	glm::vec2 screenSize = ScreenManager::Instance()->GetSize();
	//~~~~~Play Button~~~~~/
	Button* button = new Button(glm::vec2(screenSize.x / 20 , screenSize.y / 2), glm::vec2(300, 50), buttonBack, "texture");
	button->SetButtonText("Play");
	button->SetCallBack([]() { StateManager::Instance()->PushState("[STATE]Level"); 
	});
	_MenuButtons.push_back(button);
	//~~~~~Instructions Button~~~~~/
	button = new Button(glm::vec2(screenSize.x / 20, screenSize.y / 2 - 60), glm::vec2(300, 50), buttonBack, "texture");
	button->SetButtonText("Instructions");
	button->SetCallBack([]() { StateManager::Instance()->PushState("[STATE]Instructions"); });
	_MenuButtons.push_back(button);
	//~~~~~HighScore Button~~~~~/
	button = new Button(glm::vec2(screenSize.x / 20, screenSize.y / 2 - 120), glm::vec2(300, 50), buttonBack, "texture");
	button->SetButtonText("High Scores");
	button->SetCallBack([]() { StateManager::Instance()->PushState("[STATE]HighScores"); });
	_MenuButtons.push_back(button);
	//~~~~~Options Button~~~~~/
	button = new Button(glm::vec2(screenSize.x / 20, screenSize.y / 2 - 180), glm::vec2(300, 50), buttonBack, "texture");
	button->SetButtonText("Options");
	button->SetCallBack([]() { StateManager::Instance()->PushState("[STATE]Options"); });
	_MenuButtons.push_back(button);
	//~~~~~Exit Button~~~~~/
	button = new Button(glm::vec2(screenSize.x / 20, screenSize.y / 2 - 240), glm::vec2(300, 50), buttonBack, "texture");
	button->SetButtonText("Exit");
	button->SetCallBack([]() { InputManager::Instance()->RequestQuit(); });
	_MenuButtons.push_back(button);

	_SceneSky = ResourceManager::Instance()->GetSkyBox("DesertSkyBox");
	_Camera = ResourceManager::Instance()->GetCamera();
	ScreenManager::Instance()->ReleaseMouse();
	InputManager::Instance()->ReleaseMouse();

	return true;
}

void MenuState::Input()
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

void MenuState::Update(float delta)
{
	_SkyboxSpinAngle += 0.001f;
}

void MenuState::Render()
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
	_TextRenderer->RenderText("Roller Racer", glm::vec2(screenSize.x * 0.5f, screenSize.y * 0.75f), 2, true);
}

bool MenuState::Shutdown()
{
	for (auto b : _MenuButtons) {
		delete b;
	}
	_MenuButtons.clear();
	return true;
}

void MenuState::Pause()
{
	_IsPaused = true;
}

void MenuState::Resume()
{
	_IsPaused = false;
	ScreenManager::Instance()->ReleaseMouse();
	InputManager::Instance()->ReleaseMouse();
}

bool MenuState::IsPaused()
{
	return _IsPaused;
}
