#include <iostream>

#include <SDL\SDL.h>
#include <GLEW\glew.h>

#include <iostream>
#include <Windows.h>

#include "ScreenManager.h"
#include "InputManager.h"
#include "LogManager.h"

#include "Shaders\Shader.h"

int main(int argc, char** argv) {

	bool running = true;

	

	ScreenManager::Instance()->SetOpenGLVersion();
	if (!ScreenManager::Instance()->Initialize("Managed & Logged Window", 1024, 768)) {
		LogManager::Instance()->LogError("Screen Manager Failed To Initialize!");
		return 0;
	}

	Shader myShader("Shaders/shader");
	
	float vertices[] = {
		// positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	while (running) {

		ScreenManager::Instance()->Clear();

		InputManager::Instance()->Update();

		myShader.Use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		

		ScreenManager::Instance()->SwapBuffers();

		if (InputManager::Instance()->IsKeyPressed(SDLK_ESCAPE) || InputManager::Instance()->CheckForWinEvent(SDL_QUIT)) {
			running = false;
		}

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
	ScreenManager::Instance()->Close();
	return 0;
}