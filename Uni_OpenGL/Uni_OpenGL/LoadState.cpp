#include "LoadState.h"
#include "ShaderManager.h"
#include "StateManager.h"

#include "PlayState.h"


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
    ShaderManager::Instance()->AddShader("skybox", "skybox");
    ShaderManager::Instance()->AddShader("betterLight", "betterLight");


    if (!StateManager::Instance()->AddState("[STATE]Play", new PlayState())) {
        return false;
    }

    

    return true;
}

void LoadState::Input()
{
}

void LoadState::Update(float delta)
{
    if (!StateManager::Instance()->PushState("[STATE]Play")) {
    }
}

void LoadState::Render()
{
}

bool LoadState::Shutdown()
{
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
