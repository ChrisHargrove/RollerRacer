#include "LoadState.h"
#include "ShaderManager.h"


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


    return true;
}

void LoadState::Input()
{
}

void LoadState::Update(float delta)
{
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
