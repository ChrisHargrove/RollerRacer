#include "OptionsState.h"
#include "Texture.h"
#include "StateManager.h"
#include "ScreenManager.h"
#include "InputManager.h"
#include "PlayState.h"
#include "ResourceManager.h"
#include "Tools.h"

OptionsState::OptionsState()
{
}


OptionsState::~OptionsState()
{
}

bool OptionsState::Initialize()
{
	Texture* buttonBack = ResourceManager::Instance()->GetTexture("ButtonBase");
	_TextRenderer = ResourceManager::Instance()->GetTextRenderer("Font_Calibri");
	_OptionsData = ResourceManager::Instance()->GetOptions();
	glm::vec2 screenSize = ScreenManager::Instance()->GetSize();

	//~~~~~Back Button~~~~~/
	Button* button = new Button(glm::vec2(screenSize.x / 20, screenSize.y / 10), glm::vec2(300, 50), buttonBack, "texture");
	button->SetButtonText("Back");
	button->SetCallBack([this]() { StateManager::Instance()->PopState(); });
	_MenuButtons.push_back(button);

	//~~~~~ToonShadingOption~~~~~//
	RadioButton* radButton = new RadioButton(glm::vec2(screenSize.x / 20, screenSize.y * 0.75), glm::vec2(30), ResourceManager::Instance()->GetTexture("ButtonBase"),
		ResourceManager::Instance()->GetTexture("ButtonRadio"), "texture");
	radButton->SetButtonText("Toggle Toon Shading");
	radButton->SetCallBack([this]() { TurnOnToonShading(true); _OptionsData->at("ToonShading") = 1; });
	radButton->SetOtherCallBack([this]() { TurnOnToonShading(false); _OptionsData->at("ToonShading") = 0; });
	radButton->SetOn((bool)_OptionsData->at("ToonShading"));
	_RadioButtons.push_back(radButton);

	_SceneSky = ResourceManager::Instance()->GetSkyBox("DesertSkyBox");
	_Camera = ResourceManager::Instance()->GetCamera();
	ScreenManager::Instance()->ReleaseMouse();
	InputManager::Instance()->ReleaseMouse();

	return true;
}

void OptionsState::Input()
{
	if (InputManager::Instance()->IsButtonPressed(SDL_BUTTON_LEFT)) {
		for (auto b : _MenuButtons) {
			if (b->IsColliding(glm::vec2(InputManager::Instance()->GetMouseMove().xPos, InputManager::Instance()->GetMouseMove().yPos))) {
				b->CallBack();
				InputManager::Instance()->ResetMouseEvents();
			}
		}
		for (auto b : _RadioButtons) {
			if (b->IsColliding(glm::vec2(InputManager::Instance()->GetMouseMove().xPos, InputManager::Instance()->GetMouseMove().yPos))) {
				b->CallBack();
				InputManager::Instance()->ResetMouseEvents();
			}
		}
	}
}

void OptionsState::Update(float delta)
{
	_SkyboxSpinAngle += 0.001f;
}

void OptionsState::Render()
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
	for (auto r : _RadioButtons) {
		r->Render();
		ShaderManager::Instance()->GetShader("texture")->UpdateMatrices(model, _Camera->GetOrthoView(), ScreenManager::Instance()->GetProjection());
		glm::vec2 pos = r->GetPosition();
		_TextRenderer->RenderText(r->GetButtonText(), glm::vec2(pos.x + 36, pos.y), 1, false, glm::vec3(0, 0, 0));
	}
	glm::vec2 screenSize = ScreenManager::Instance()->GetSize();
	_TextRenderer->RenderText("Options", glm::vec2(screenSize.x * 0.5f, screenSize.y * 0.85f), 2, true);
	
}

bool OptionsState::Shutdown()
{
	for (auto b : _MenuButtons) {
		delete b;
	}
	_MenuButtons.clear();
	for (auto r : _RadioButtons) {
		delete r;
	}
	_RadioButtons.clear();
	return true;
}

void OptionsState::Pause()
{
	_IsPaused = true;
}

void OptionsState::Resume()
{
	_IsPaused = false;
}

bool OptionsState::IsPaused()
{
	return _IsPaused;
}

void OptionsState::TurnOnToonShading(bool turnOn)
{
	ShaderManager::Instance()->GetShader("terrain")->SetBool("toonShadingOn", turnOn);
	ShaderManager::Instance()->GetShader("betterLight")->SetBool("toonShadingOn", turnOn);
}
