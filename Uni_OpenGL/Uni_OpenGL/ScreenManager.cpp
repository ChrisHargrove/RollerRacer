////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "ScreenManager.h"
#include "LogManager.h"

////////////////////////////////////////////////////////////
// Static Variables
////////////////////////////////////////////////////////////
ScreenManager ScreenManager::_Instance;

////////////////////////////////////////////////////////////
ScreenManager::ScreenManager() :
	_VersionSet(false)
{}

////////////////////////////////////////////////////////////
ScreenManager::~ScreenManager(){}

////////////////////////////////////////////////////////////
bool ScreenManager::Initialize(char* Title, int Width, int Height, bool CoreMode)
{
	LogManager::Instance()->LogDebug("Initializing ScreenManager...");
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		std::string temp = SDL_GetError();
		LogManager::Instance()->LogError("SDL_Init() Failed: " + temp);
		return false;
	}

	if (!_VersionSet) {
		LogManager::Instance()->LogError("SetOpenGLVersion() Not called! - Cannot Initialize Window without OpenGL version to use.");
		return false;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, _GL_VERSION_MAJOR);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, _GL_VERSION_MINOR);

	
	//Set SDL_GL Attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	//Set SDL Double Buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if (CoreMode) {
		//Set OpenGL Core mode
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		LogManager::Instance()->LogDebug("OpenGL Set to Core Profile");
	}
	else {
		//Set OpenGL Comapatability mode
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
		LogManager::Instance()->LogDebug("OpenGL Set to Compatability Profile");
	}

	SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//Create a Window
	_Window = SDL_CreateWindow(Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_OPENGL);
	if (_Window == 0) {
		std::string temp = SDL_GetError();
		LogManager::Instance()->LogError("SDL_CreateWindow() Failed: " + temp);
		return false;
	}
	LogManager::Instance()->LogInfo("Window Created: " + std::to_string(Width) + " x " + std::to_string(Height));
	//Create OpenGL Context
	_Context = SDL_GL_CreateContext(_Window);
	if (_Context == NULL) {
		std::string temp = SDL_GetError();
		LogManager::Instance()->LogError("SDL_GL_CreateContext() Failed: " + temp);
		return false;
	}
	
	EnableVSync(); //Note: Has to be called after Context has been created!

	GLenum glew_error = glewInit();
	if (GLEW_OK != glew_error) {
		std::string temp; 
		temp = (char*)glewGetErrorString(glew_error);
		LogManager::Instance()->LogError("glewInit() Failed:" + temp);
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////
void ScreenManager::Close()
{
	LogManager::Instance()->LogDebug("Closing ScreenManager...");
	SDL_GL_DeleteContext(_Context);	// Free memory to the context
	SDL_DestroyWindow(_Window);		// Free memory to the window
	SDL_Quit();						// Close all SDL subsystems
}

////////////////////////////////////////////////////////////
void ScreenManager::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

////////////////////////////////////////////////////////////
void ScreenManager::SwapBuffers()
{
	SDL_GL_SwapWindow(_Window);
}

////////////////////////////////////////////////////////////
bool ScreenManager::EnableVSync(bool VSync)
{
	if (VSync) {
		if (SDL_GL_SetSwapInterval(1) == 0) {
			LogManager::Instance()->LogInfo("VSync Enabled.");
			return true;
		}
	}
	else {
		if (SDL_GL_SetSwapInterval(0) == 0) {
			LogManager::Instance()->LogInfo("VSync Disabled.");
			return true;
		}
	}
	LogManager::Instance()->LogWarning("EnableVSync() - Swap Interval Not Supported.");
	return false;
}

////////////////////////////////////////////////////////////
void ScreenManager::SetClearColor(float Red, float Green, float Blue, float Alpha)
{
	glClearColor(Red, Green, Blue, Alpha);
}

////////////////////////////////////////////////////////////
void ScreenManager::SetOpenGLVersion(int Major, int Minor)
{
	_GL_VERSION_MAJOR = Major;
	_GL_VERSION_MINOR = Minor;
	_VersionSet = true;
	LogManager::Instance()->LogInfo("OpenGL Version Set To: " + std::to_string(Major) + "." + std::to_string(Minor));
}

////////////////////////////////////////////////////////////
SDL_Window * ScreenManager::GetWindow()
{
	return _Window;
}

////////////////////////////////////////////////////////////
SDL_GLContext * ScreenManager::GetContext()
{
	return &_Context;
}
