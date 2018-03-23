////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Engine.h"
#include "LoadState.h"
#include "LogManager.h"
#include "StateManager.h"
#include "InputManager.h"
#include "ScreenManager.h"


Engine::Engine()
{
    LogManager::Instance()->LogInfo("Creating Game Engine...");
}

Engine::~Engine()
{
}

bool Engine::Initialize(int Width, int Height, std::string Title)
{
    ScreenManager::Instance()->SetOpenGLVersion();
    if (!ScreenManager::Instance()->Initialize(Title.c_str(), Width, Height)) {
        LogManager::Instance()->LogError("Screen Manager Failed To Initialize!");
        return false;
    }

    if (!StateManager::Instance()->AddState("[STATE]Load", new LoadState())) {
        return false;
    }
    if (!StateManager::Instance()->ChangeState("[STATE]Load")) {
        return false;
    }

    return true;
}

void Engine::Input()
{
    InputManager::Instance()->Update();
    StateManager::Instance()->Input();
}

void Engine::Update(float delta)
{
    StateManager::Instance()->Update(delta);
}

void Engine::Render()
{
    ScreenManager::Instance()->Clear();
    StateManager::Instance()->Render();
    ScreenManager::Instance()->SwapBuffers();
}

bool Engine::Shutdown()
{
    if (!StateManager::Instance()->Shutdown()) {
        LogManager::Instance()->LogError("Engine Shutdown Failed! Engine.cpp - Line 53");
        return false;
    }

    ScreenManager::Instance()->Close();
    return true;
}
