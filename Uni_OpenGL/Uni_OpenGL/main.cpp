#include <iostream>

#include <SDL\SDL.h>
#include <GLEW\glew.h>

#include <iostream>
#include <Windows.h>

#include "ScreenManager.h"
#include "LogManager.h"

int main(int argc, char** argv) {

	ScreenManager::Instance()->SetOpenGLVersion();
	if (!ScreenManager::Instance()->Initialize("Managed & Logged Window", 1024, 768, false)) {
		LogManager::Instance()->LogError("Screen Manager Failed To Initialize!");
		return 0;
	}
	
	float time = 0;

	while (1) {

		ScreenManager::Instance()->Clear();

		time+=0.01f;
		float offset = sin(time);

		//render quad
		glBegin(GL_QUADS);
			glColor3f(1.0f * offset, 0.0f, 0.0f);
			glVertex3f(-0.5f + offset, 0.5f, 0.0f);

			glColor3f(0.0f, 1.0f * offset, 0.0f);
			glVertex3f(0.5f + offset, 0.5f, 0.0f);

			glColor3f(0.0f, 0.0f, 1.0f * offset);
			glVertex3f(0.5f + offset, -0.5f, 0.0f);

			glColor3f(1.0f * offset, 0.0f, 1.0f * offset);
			glVertex3f(-0.5f + offset, -0.5f, 0.0f);
		glEnd();

		ScreenManager::Instance()->SwapBuffers();

	}
	
	ScreenManager::Instance()->Close();
	return 0;
}