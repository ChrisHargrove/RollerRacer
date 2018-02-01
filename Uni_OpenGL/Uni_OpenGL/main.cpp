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
#include "Buffer.h"

#include "Grid.h"
#include "Axis.h"
#include "Cuboid.h"

#include "Texture.h"

#include <SDL\SDL_image.h>

int main(int argc, char** argv) {

	ScreenManager::Instance()->SetOpenGLVersion();
	if (!ScreenManager::Instance()->Initialize("Managed & Logged Window", 1024, 768)) {
		LogManager::Instance()->LogError("Screen Manager Failed To Initialize!");
		return 0;
	}

	ShaderManager::Instance()->AddShader("basic", "basic");
	ShaderManager::Instance()->AddShader("phong", "phong");
    ShaderManager::Instance()->AddShader("betterLight", "betterLight");
	
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    // positions all containers
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	Buffer VertexArray, VertexBuffer;

	VertexArray.Create(VAO);

	VertexBuffer.Create(VBO);

	VertexArray.Bind();
	VertexBuffer.Fill(sizeof(vertices),vertices, STATIC);
	VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader("betterLight")->GetID(), "aPos", 3, VT_FLOAT, 8 * sizeof(float));
	VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader("betterLight")->GetID(), "aNormal", 3, VT_FLOAT, 8 * sizeof(float), 3 * sizeof(float));
    VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader("betterLight")->GetID(), "aTexCoords", 3, VT_FLOAT, 8 * sizeof(float), 6 * sizeof(float));
	
	Grid* grid = new Grid(7, "basic");
	Axis* axis = new Axis(7, "basic");
	Cuboid* lamp = new Cuboid(0.5, 0.5, 0.5, "phong");
	
	Camera* camera = new Camera(0, 1, 4, -90);

    Texture* container = new Texture("container.png");
    Texture* containerSpec = new Texture("container_specular.png");
	
    ShaderManager::Instance()->GetShader("betterLight")->SetInt("material.diffuse", 0);
    ShaderManager::Instance()->GetShader("betterLight")->SetInt("material.specular", 1);

	while (!InputManager::Instance()->HasQuit()) {
		ScreenManager::Instance()->SetProjection(90, camera->GetZoom());

		ScreenManager::Instance()->Clear();
		InputManager::Instance()->Update();

		camera->Update();

		//------------BETTER LIGHTING SHADER----------
        ShaderManager::Instance()->GetShader("betterLight")->SetFloat("material.shininess", 64.0f);
        //------------DIRECTION LIGHT-----------------
        ShaderManager::Instance()->GetShader("betterLight")->SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        ShaderManager::Instance()->GetShader("betterLight")->SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        ShaderManager::Instance()->GetShader("betterLight")->SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        ShaderManager::Instance()->GetShader("betterLight")->SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        //------------POINT LIGHTS--------------------
        for (int i = 0; i < 4; i++) {
            ShaderManager::Instance()->GetShader("betterLight")->SetVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
            ShaderManager::Instance()->GetShader("betterLight")->SetVec3("pointLights[" + std::to_string(i) + "].ambient", 0.05f, 0.05f, 0.05f);
            ShaderManager::Instance()->GetShader("betterLight")->SetVec3("pointLights[" + std::to_string(i) + "].diffuse", 0.8f, 0.8f, 0.8f);
            ShaderManager::Instance()->GetShader("betterLight")->SetVec3("pointLights[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
            ShaderManager::Instance()->GetShader("betterLight")->SetFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
            ShaderManager::Instance()->GetShader("betterLight")->SetFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
            ShaderManager::Instance()->GetShader("betterLight")->SetFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
        }
        //------------SPOT LIGHT----------------------
        ShaderManager::Instance()->GetShader("betterLight")->SetVec3("spotLight.position", camera->GetPosition());
        ShaderManager::Instance()->GetShader("betterLight")->SetVec3("spotLight.direction", camera->GetDirection());
        ShaderManager::Instance()->GetShader("betterLight")->SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        ShaderManager::Instance()->GetShader("betterLight")->SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        ShaderManager::Instance()->GetShader("betterLight")->SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        ShaderManager::Instance()->GetShader("betterLight")->SetFloat("spotLight.constant", 1.0f);
        ShaderManager::Instance()->GetShader("betterLight")->SetFloat("spotLight.linear", 0.09);
        ShaderManager::Instance()->GetShader("betterLight")->SetFloat("spotLight.quadratic", 0.032);
        ShaderManager::Instance()->GetShader("betterLight")->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        ShaderManager::Instance()->GetShader("betterLight")->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		glm::mat4 model;
		ShaderManager::Instance()->GetShader("betterLight")->UpdateMatrices(model, camera->GetViewMatrix(), ScreenManager::Instance()->GetProjection());
        ShaderManager::Instance()->GetShader("phong")->SetVec3("light.ambient", 1.0f, 1.0f, 1.0f);
        ShaderManager::Instance()->GetShader("phong")->SetVec3("material.ambient", 1.0f, 1.0f, 1.0f);

        //activate + bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, container->GetID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, containerSpec->GetID());

		VertexArray.Bind();
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            model = glm::mat4();
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ShaderManager::Instance()->GetShader("betterLight")->UpdateMatrices(model, camera->GetViewMatrix(), ScreenManager::Instance()->GetProjection());

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        for (int i = 0; i < 4; i++) {
            model = glm::mat4();
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            ShaderManager::Instance()->GetShader("phong")->UpdateMatrices(model, camera->GetViewMatrix(), ScreenManager::Instance()->GetProjection());
            lamp->Draw();
        }
		

		//------------BASIC SHADER----------
		model = glm::mat4();
		ShaderManager::Instance()->GetShader("basic")->UpdateMatrices(model, camera->GetViewMatrix(), ScreenManager::Instance()->GetProjection());
		grid->Draw();
		axis->Draw();

		ScreenManager::Instance()->SwapBuffers();
	}

	VertexArray.Destroy();
	VertexBuffer.Destroy();

	delete camera;
	delete axis;
	delete lamp;
	delete grid;
    delete container;

	ScreenManager::Instance()->Close();

	return 0;
}