#include "PlayState.h"

#include "ScreenManager.h"
#include "InputManager.h"
#include "Texture.h"

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

    _SceneSky= new SkyBox("skybox", "Assets/Textures/hw_sahara/D", "Dskybox");

    _Camera = new Camera(0, 1, 4, -90);

	_DirectionalLight = new DirectionalLight(glm::vec3(0, -0.5, -0.5), glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.4, 0.4, 0.4), glm::vec3(1, 1, 1));
	_PointLight = new PointLight(glm::vec3(0, 5, 3), 1.0,	0.045,	0.0075, glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.4, 0.4, 0.4), glm::vec3(1, 1, 1));

    Texture* texture = new Texture("Assets/container.png", "temp");

	test = new Model("Assets/Models/nanosuit/nanosuit.obj", "betterLight");

    return true;
}

void PlayState::Input()
{

}

void PlayState::Update(float delta)
{
    _Camera->Update();
}

void PlayState::Render()
{
    //~~~~~~~ALL 3D RENDERING~~~~~~//
    ScreenManager::Instance()->Set3D(90, _Camera->GetZoom(), 0.1, 1000.0f);

    glm::mat4 model = glm::mat4();
    ShaderManager::Instance()->GetShader("basic")->UpdateMatrices(model, _Camera->GetViewMatrix(), ScreenManager::Instance()->GetProjection());
    //_SceneGrid->Render();
    _SceneAxis->Render();
    //ShaderManager::Instance()->GetShader("basic")->SetVec3("aColor", glm::vec3(1.0, 1.0, 1.0));
    ShaderManager::Instance()->GetShader("betterLight")->UpdateMatrices(model, _Camera->GetViewMatrix(), ScreenManager::Instance()->GetProjection());
	_DirectionalLight->SendToShader("betterLight");
	_PointLight->SendToShader(0, "betterLight");
    ShaderManager::Instance()->GetShader("betterLight")->SetVec3("viewPos", _Camera->GetPosition());
	test->Render();

    ShaderManager::Instance()->GetShader("skybox")->UpdateMatrices(model, _Camera->GetViewMatrix(), ScreenManager::Instance()->GetProjection());
    _SceneSky->Render();

    //~~~~~~~ALL 2D RENDERING~~~~~~//
    ScreenManager::Instance()->Set2D();
}

bool PlayState::Shutdown()
{
    delete _SceneAxis;
    delete _SceneGrid;
    delete _SceneSky;
    delete _Camera;

    return true;
}

void PlayState::Pause()
{
    _IsPaused = true;
}

void PlayState::Resume()
{
    _IsPaused = false;
}

bool PlayState::IsPaused()
{
    return _IsPaused;
}
