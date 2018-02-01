////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "InputManager.h"
#include "ScreenManager.h"
#include "LogManager.h"
#include <SDL\SDL.h>

////////////////////////////////////////////////////////////
// Static Variables
////////////////////////////////////////////////////////////
InputManager InputManager::_Instance;

////////////////////////////////////////////////////////////
InputManager::InputManager() : _CapturedMouse(true){}

////////////////////////////////////////////////////////////
InputManager::~InputManager(){}

////////////////////////////////////////////////////////////
void InputManager::Update()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_KEYDOWN:
			AddKeyboardEvent(e.key.keysym.sym, e.key.state, e.key.keysym.mod);
			break;
		case SDL_KEYUP:
			AddKeyboardEvent(e.key.keysym.sym, e.key.state, e.key.keysym.mod);
			break;
		case SDL_WINDOWEVENT:
			AddWindowEvent(e.window.windowID, e.window.event, e.window.data1, e.window.data2);
			break;
		case SDL_MOUSEBUTTONDOWN:
			AddMouseButtonEvent(e.button.button, e.button.state, e.button.clicks);
			break;
		case SDL_MOUSEBUTTONUP:
			AddMouseButtonEvent(e.button.button, e.button.state, e.button.clicks);
			break;
		case SDL_MOUSEMOTION:
			AddMouseMoveEvent(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
			break;
		case SDL_MOUSEWHEEL:
			AddMouseScrollEvent(e.wheel.x, e.wheel.y, e.wheel.direction);
			break;
		case SDL_QUIT:
			AddWindowEvent(e.window.windowID, e.type, 0, 0);
			break;
		}
	}
}

////////////////////////////////////////////////////////////
bool InputManager::IsKeyPressed(int Key)
{
	auto search = KeyboardEvents.find(Key);
	if (search != KeyboardEvents.end()) {
		if (KeyboardEvents.at(Key).State == SDL_PRESSED && KeyPressed.at(Key) != true) {
			KeyPressed.at(Key) = true;
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////
bool InputManager::IsKeyReleased(int Key)
{
	auto search = KeyboardEvents.find(Key);
	if (search != KeyboardEvents.end()) {
		if (KeyboardEvents.at(Key).State == SDL_RELEASED) {
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////
bool InputManager::IsKeyHeld(int Key)
{
	auto search = KeyboardEvents.find(Key);
	if (search != KeyboardEvents.end()) {
		if (KeyboardEvents.at(Key).State == SDL_PRESSED && KeyPressed.at(Key) == true) {
			return true;
		}
		else if (KeyboardEvents.at(Key).State == SDL_PRESSED) {
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////
bool InputManager::IsButtonPressed(int Button)
{
	auto search = MouseButtonEvents.find(Button);
	if (search != MouseButtonEvents.end()) {
		if (MouseButtonEvents.at(Button).State == SDL_PRESSED) {
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////
bool InputManager::IsButtonReleased(int Button)
{
	auto search = MouseButtonEvents.find(Button);
	if (search != MouseButtonEvents.end()) {
		if (MouseButtonEvents.at(Button).State == SDL_RELEASED) {
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////
bool InputManager::HasMouseMoved()
{
	bool r = MouseMoveEvt.HasMoved;
	MouseMoveEvt.HasMoved = false;
	return r;
}

////////////////////////////////////////////////////////////
const MouseMoveEvent InputManager::GetMouseMove()
{
	return MouseMoveEvt;
}

////////////////////////////////////////////////////////////
bool InputManager::HasScrolled()
{
	bool r = MouseScrollEvt.HasMoved;
	MouseScrollEvt.HasMoved = false;
	return r;
}

////////////////////////////////////////////////////////////
double InputManager::GetXScroll()
{
	return MouseScrollEvt.X;
}

////////////////////////////////////////////////////////////
double InputManager::GetYScroll()
{
	return MouseScrollEvt.Y;
}

////////////////////////////////////////////////////////////
bool InputManager::HasQuit()
{
    if (IsKeyPressed(SDLK_ESCAPE) || CheckForWinEvent(SDL_QUIT)) {
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////
bool InputManager::CheckForWinEvent(int WindowEvent)
{
	auto search = WindowEvents.find(WindowEvent);
	if (search != WindowEvents.end()) {
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////
WindowEvent InputManager::GetWinEvent(int WindowEvent)
{
	auto search = WindowEvents.find(WindowEvent);
	if (search != WindowEvents.end()) {
		return WindowEvents.at(WindowEvent);
	}
}

////////////////////////////////////////////////////////////
void InputManager::GrabMouse()
{
	if (!_CapturedMouse) {
		ScreenManager::Instance()->GrabMouse();
		_CapturedMouse = true;
	}
}

////////////////////////////////////////////////////////////
void InputManager::ReleaseMouse()
{
	if (_CapturedMouse) {
		ScreenManager::Instance()->ReleaseMouse();
		_CapturedMouse = false;
	}
}

////////////////////////////////////////////////////////////
void InputManager::AddKeyboardEvent(int Keycode, int State, int Mod)
{
	KeyEvent kEvent;
	kEvent.Keycode = Keycode;
	kEvent.State = State;
	kEvent.Mod = Mod;

	auto search = KeyboardEvents.find(Keycode);
	if (search != KeyboardEvents.end()) {
		KeyboardEvents.at(Keycode) = kEvent;
		if (State == SDL_RELEASED) {
			KeyPressed.at(Keycode) = false;
		}
	}
	else {
		KeyboardEvents.emplace(std::make_pair(Keycode, kEvent));
		KeyPressed.emplace(std::make_pair(Keycode, false));
	}
}

////////////////////////////////////////////////////////////
void InputManager::AddWindowEvent(int WindowID, int Type, int Data1, int Data2)
{
	WindowEvent wEvent;
	wEvent.WindowID = WindowID;
	wEvent.Type = Type;
	wEvent.Data1 = Data1;
	wEvent.Data2 = Data2;

	auto search = WindowEvents.find(Type);
	if (search != WindowEvents.end()) {
		WindowEvents.at(Type) = wEvent;
	}
	else {
		WindowEvents.emplace(std::make_pair(Type, wEvent));
	}
}

////////////////////////////////////////////////////////////
void InputManager::AddMouseScrollEvent(int X, int Y, int Direction)
{
	MouseScrollEvt.X = X;
	MouseScrollEvt.Y = Y;
	MouseScrollEvt.Direction = Direction;
	if (Direction == SDL_MOUSEWHEEL_FLIPPED) {
		MouseScrollEvt.X *= -1;
		MouseScrollEvt.Y *= -1;
	}
	MouseScrollEvt.HasMoved = true;
}

////////////////////////////////////////////////////////////
void InputManager::AddMouseMoveEvent(int xPos, int yPos, int xRel, int yRel)
{
	MouseMoveEvt.xPos = xPos;
	MouseMoveEvt.yPos = yPos;
	MouseMoveEvt.xRel = xRel;
	MouseMoveEvt.yRel = yRel;
	MouseMoveEvt.HasMoved = true;
}

////////////////////////////////////////////////////////////
void InputManager::AddMouseButtonEvent(int Button, int State, int Clicks)
{
	MouseButtonEvent mEvent;
	mEvent.Button = Button;
	mEvent.State = State;
	mEvent.Clicks = Clicks;

	auto search = MouseButtonEvents.find(Button);
	if (search != MouseButtonEvents.end()) {
		MouseButtonEvents.at(Button) = mEvent;
	}
	else {
		MouseButtonEvents.emplace(std::make_pair(Button, mEvent));
	}
}
