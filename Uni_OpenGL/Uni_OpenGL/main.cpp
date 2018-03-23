#include <iostream>

#include <SDL\SDL.h>
#include <GLEW\glew.h>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>

#include <Windows.h>

#include "ScreenManager.h"
#include "InputManager.h"
#include "LogManager.h"
#include "ShaderManager.h"

#include "Camera.h"

#include "Grid.h"
#include "Axis.h"

#include "Skybox.h"
#include "Model.h"
#include "Texture.h"

#include <SDL\SDL_image.h>

int main(int argc, char** argv) {

	ScreenManager::Instance()->SetOpenGLVersion();
	if (!ScreenManager::Instance()->Initialize("Managed & Logged Window", 1024, 768)) {
		LogManager::Instance()->LogError("Screen Manager Failed To Initialize!");
		return 0;
	}

	ShaderManager::Instance()->AddShader("basic", "basic");
    ShaderManager::Instance()->AddShader("skybox", "skybox");
    ShaderManager::Instance()->AddShader("betterLight", "betterLight");
	
	Grid* grid = new Grid(7, "basic");
	Axis* axis = new Axis(7, "basic");

    SkyBox* skybox = new SkyBox("skybox", "Assets/Textures/hw_sahara/D", "Dskybox");
    Model* carModel = new Model("Assets/Models/nanosuit/nanosuit.obj", "betterLight");
	
	Camera* camera = new Camera(0, 1, 4, -90);

	while (!InputManager::Instance()->HasQuit()) {

		ScreenManager::Instance()->Set3D(90, camera->GetZoom(), 0.1, 1000.0f);

		ScreenManager::Instance()->Clear();
		InputManager::Instance()->Update();

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

		ScreenManager::Instance()->SwapBuffers();
	}

	delete camera;
	delete axis;
	delete grid;
    delete skybox;

	ScreenManager::Instance()->Close();

	return 0;
}