#include <SDL\SDL.h>
#include "Engine.h"

int main(int argc, char** argv) {

    Engine* _Engine = new Engine();

    if (!_Engine->Initialize(1920, 1080, "Finite State Machine!!!")) {
        return 0;
    }

    return _Engine->Run();

}