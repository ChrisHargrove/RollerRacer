#include <iostream>

#include <SDL\SDL.h>
#include <GLEW\glew.h>

SDL_Window* window;
SDL_GLContext context;

float time;

int main(int argc, char** argv) {

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		std::cout << "SDL_Init() Failed Check line 11" << std::endl;
	}

	glewInit();

	//Set SDL_GL Attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	//Set SDL Double Buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//Set Clear Color
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//Set OpenGL Core mode
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//Set OpenGL Comapatability mode
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	//Set OpenGL Context version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	//Setup Vsync
	SDL_GL_SetSwapInterval(1);

	//Create a window
	window = SDL_CreateWindow("My Laggy Ass Window!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);
	if (!window) {
		std::cout << "SDL_CreateWindow() Failed Check line 36" << std::endl;
	}
	context = SDL_GL_CreateContext(window);
	if (!context) {
		std::cout << "SDL_GL_CreateContext() Failed Check line 40" << std::endl;
	}

	time = 0;

	while (1) {
		//clear screen
		glClear(GL_COLOR_BUFFER_BIT);

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


		//swap buffers
		SDL_GL_SwapWindow(window);
	}

	//Destroy Window Resources
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}