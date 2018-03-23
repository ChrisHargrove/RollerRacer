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
	return 1;
}
	/*Grid* grid = new Grid(7, "basic");
	Axis* axis = new Axis(7, "basic");

    SkyBox* skybox = new SkyBox("skybox", "Assets/Textures/hw_sahara/D", "Dskybox");
    Model* carModel = new Model("Assets/Models/nanosuit/nanosuit.obj", "betterLight");
	
	Camera* camera = new Camera(0, 1, 4, -90);

		ScreenManager::Instance()->Set3D(90, camera->GetZoom(), 0.1, 1000.0f);
		camera->Update();

		//------------BASIC SHADER----------
		glm::mat4 model = glm::mat4();
		ShaderManager::Instance()->GetShader("basic")->UpdateMatrices(model, camera->GetViewMatrix(), ScreenManager::Instance()->GetProjection());
		grid->Draw();
		axis->Draw();

        ShaderManager::Instance()->GetShader("betterLight")->UpdateMatrices(model, camera->GetViewMatrix(), ScreenManager::Instance()->GetProjection());
        carModel->Draw();

        ShaderManager::Instance()->GetShader("skybox")->UpdateMatrices(model, glm::mat4(glm::mat3(camera->GetViewMatrix())), ScreenManager::Instance()->GetProjection());
        skybox->Draw();

        //-----------DRAW ALL GUI STUFF---------
		//DRAW ALL 2D stuff!
		ScreenManager::Instance()->Set2D();

		
	}

	delete camera;
	delete axis;
	delete grid;
    delete skybox;

	*/
