#include "InstructionsState.h"
#include "Texture.h"
#include "StateManager.h"
#include "ScreenManager.h"
#include "InputManager.h"
#include "PlayState.h"
#include "ResourceManager.h"


InstructionsState::InstructionsState()
{
}


InstructionsState::~InstructionsState()
{
}

bool InstructionsState::Initialize()
{
	Texture* buttonBack = ResourceManager::Instance()->GetTexture("ButtonBase");
	_TextRenderer = ResourceManager::Instance()->GetTextRenderer("Font_Calibri");

	glm::vec2 screenSize = ScreenManager::Instance()->GetSize();
	//~~~~~Back Button~~~~~/
	Button* button = new Button(glm::vec2(screenSize.x / 20 , screenSize.y / 10), glm::vec2(300, 50), buttonBack, "texture");
	button->SetButtonText("Back");
	button->SetCallBack([]() {
		StateManager::Instance()->PopState();
	});
	_MenuButtons.push_back(button);

	_SceneSky = ResourceManager::Instance()->GetSkyBox("DesertSkyBox");
	_Camera = ResourceManager::Instance()->GetCamera();
	ScreenManager::Instance()->ReleaseMouse();
	InputManager::Instance()->ReleaseMouse();

	return true;
}

void InstructionsState::Input()
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

void InstructionsState::Update(float delta)
{
	_SkyboxSpinAngle += 0.001f;
}

void InstructionsState::Render()
{
	//~~~~~~~ALL 3D RENDERING~~~~~~//
	ScreenManager::Instance()->Set3D(90.0f, _Camera->GetZoom(), 0.1f, 1000.0f);

	glm::vec2 screenSize = ScreenManager::Instance()->GetSize();

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
	_TextRenderer->RenderText("Instructions", glm::vec2(screenSize.x * 0.5f, screenSize.y * 0.85), 2, true);
	_TextRenderer->RenderText("Arrow Keys to control the car movement:", glm::vec2(screenSize.x * 0.5f, (screenSize.y * 0.7)), 0.5f, true, glm::vec3(0, 0, 0));
	_TextRenderer->RenderText("Up -> Accelerate", glm::vec2(screenSize.x * 0.5f, (screenSize.y * 0.7) - 50), 0.5f, true, glm::vec3(0, 0, 0));
	_TextRenderer->RenderText("Down -> Brake", glm::vec2(screenSize.x * 0.5f, (screenSize.y * 0.7) - 100), 0.5f, true, glm::vec3(0, 0, 0));
	_TextRenderer->RenderText("Left/Right -> Steering, respectively", glm::vec2(screenSize.x * 0.5f, (screenSize.y * 0.7) - 150), 0.5f, true, glm::vec3(0, 0, 0));
	_TextRenderer->RenderText("BackSpace -> Reset Car to Track", glm::vec2(screenSize.x * 0.5f, (screenSize.y * 0.7) - 200), 0.5f, true, glm::vec3(0, 0, 0));
	_TextRenderer->RenderText("Escape -> Pause Game", glm::vec2(screenSize.x * 0.5f, (screenSize.y * 0.7) - 250), 0.5f, true, glm::vec3(0, 0, 0));

	_TextRenderer->RenderText("The aim of the game is to race around the track, trying to get the best time possible.", glm::vec2(screenSize.x * 0.5f, (screenSize.y * 0.7) - 300), 0.5f, true, glm::vec3(0, 0, 0));
}

bool InstructionsState::Shutdown()
{
	for (auto b : _MenuButtons) {
		delete b;
	}
	_MenuButtons.clear();
	return true;
}

void InstructionsState::Pause()
{
	_IsPaused = true;
}

void InstructionsState::Resume()
{
	_IsPaused = false;
	ScreenManager::Instance()->ReleaseMouse();
	InputManager::Instance()->ReleaseMouse();
}

bool InstructionsState::IsPaused()
{
	return _IsPaused;
}
