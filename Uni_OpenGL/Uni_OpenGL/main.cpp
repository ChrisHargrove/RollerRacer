#include "Timer.h"
#include "Engine.h"
#include "InputManager.h"

int main(int argc, char** argv) {

    Timer* _Timer = new Timer();
    Engine* _Engine = new Engine();

    if (!_Engine->Initialize(1024, 768, "Finite State Machine!!!")) {
        return 0;
    }

    _Timer->Start();
    while (!InputManager::Instance()->HasQuit()) {
        _Engine->Input();
        _Engine->Update(_Timer->GetDelta());
        _Engine->Render();
    }

    if (!_Engine->Shutdown()) {
        return 0;
    }
	

    delete _Timer;
    delete _Engine;
	return 1;
}