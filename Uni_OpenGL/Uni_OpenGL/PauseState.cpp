#include "PauseState.h"

#include "StateManager.h"
#include "InputManager.h"
#include "ScreenManager.h"
#include "ResourceManager.h"

PauseState::PauseState()
{
}


PauseState::~PauseState()
{
}

bool PauseState::Initialize()
{
	Texture* buttonBack = ResourceManager::Instance()->GetTexture("ButtonBase");
	_TextRenderer = ResourceManager::Instance()->GetTextRenderer("Font_Calibri");
	glm::vec2 screenSize = ScreenManager::Instance()->GetSize();
	//~~~~~Resume Button~~~~~/
	Button* button = new Button(glm::vec2(screenSize.x / 20, screenSize.y / 2), glm::vec2(300, 50), buttonBack, "texture");
	button->SetButtonText("Resume");
	button->SetCallBack([]() { StateManager::Instance()->PopState(); });
	_MenuButtons.push_back(button);
	//~~~~~Change Track Button~~~~~/
	button = new Button(glm::vec2(screenSize.x / 20, screenSize.y / 2 - 60), glm::vec2(300, 50), buttonBack, "texture");
	button->SetButtonText("Change Track");
	button->SetCallBack([]() { StateManager::Instance()->PopBackToState("[STATE]Level"); });
	_MenuButtons.push_back(button);
	//~~~~~Options Button~~~~~/
	button = new Button(glm::vec2(screenSize.x / 20, screenSize.y / 2 - 120), glm::vec2(300, 50), buttonBack, "texture");
	button->SetButtonText("Options");
	button->SetCallBack([]() { StateManager::Instance()->PushState("[STATE]Options"); });
	_MenuButtons.push_back(button);
	//~~~~~Instructions Button~~~~~/
	button = new Button(glm::vec2(screenSize.x / 20, screenSize.y / 2 - 180), glm::vec2(300, 50), buttonBack, "texture");
	button->SetButtonText("Instructions");
	button->SetCallBack([]() { StateManager::Instance()->PushState("[STATE]Instructions"); });
	_MenuButtons.push_back(button);
	//~~~~~Quit Button~~~~~/
	button = new Button(glm::vec2(screenSize.x / 20, screenSize.y / 2 - 240), glm::vec2(300, 50), buttonBack, "texture");
	button->SetButtonText("Quit");
	button->SetCallBack([]() { StateManager::Instance()->PopBackToState("[STATE]Menu"); });
	_MenuButtons.push_back(button);

	_SceneSky = ResourceManager::Instance()->GetSkyBox("DesertSkyBox");
	_Camera = ResourceManager::Instance()->GetCamera();

	ScreenManager::Instance()->ReleaseMouse();
	InputManager::Instance()->ReleaseMouse();

	return true;
}

void PauseState::Input()
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

void PauseState::Update(float delta)
{
	_SkyboxSpinAngle += 0.001f;
}

void PauseState::Render()
{
	//~~~~~~~ALL 3D RENDERING~~~~~~//
	ScreenManager::Instance()->Set3D(90.0f, _Camera->GetZoom(), 0.1f, 1000.0f);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, _SkyboxSpinAngle, glm::vec3(0, 1, 0));
	ShaderManager::Instance()->GetShader("skybox")->UpdateMatrices(model, _Camera->GetViewMatrix(), ScreenManager::Instance()->GetProjection());
	_SceneSky->Render();

	//~~~~~~~ALL 2D RENDERING~~~~~~//
	ScreenManager::Instance()->Set2D();
	glm::vec2 screenSize = ScreenManager::Instance()->GetSize();
	model = glm::mat4(1.0f);
	ShaderManager::Instance()->GetShader("texture")->UpdateMatrices(model, _Camera->GetOrthoView(), ScreenManager::Instance()->GetProjection());
	for (auto b : _MenuButtons) {
		b->Render();
		ShaderManager::Instance()->GetShader("texture")->UpdateMatrices(model, _Camera->GetOrthoView(), ScreenManager::Instance()->GetProjection());
		glm::vec2 pos = b->GetPosition();
		_TextRenderer->RenderText(b->GetButtonText(), glm::vec2(pos.x + 6, pos.y + 10), 1);
	}
	_TextRenderer->RenderText("Paused", glm::vec2(screenSize.x * 0.5f, screenSize.y * 0.75f), 2, true);
}

bool PauseState::Shutdown()
{
	for (auto b : _MenuButtons) {
		delete b;
	}
	_MenuButtons.clear();
	return true;
}

void PauseState::Pause()
{
	//Useless on the Pause State xD But must be inherited
}

void PauseState::Resume()
{
	//Useless on the Pause State xD But must be inherited
}

bool PauseState::IsPaused()
{
	return false;
}
