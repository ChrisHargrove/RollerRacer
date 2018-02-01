#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <SDL\SDL.h>

#include "InputManager.h"
#include "ScreenManager.h"
#include "LogManager.h"


Camera::Camera(glm::vec3 Position, float Yaw, float Pitch, float Roll)
{
	_Position = Position;
	_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	_Yaw = Yaw;
	_Pitch = Pitch;
	_Roll = Roll;
	_Zoom = 0;
	_Speed = 0.1f;
	_ConstrainPitch = true;

	UpdateCameraVectors();
	ScreenManager::Instance()->GrabMouse();
	LogManager::Instance()->LogDebug("Creating Camera at: X:" + std::to_string(_Position.x) + " Y:" + std::to_string(_Position.y) + " Z:" + std::to_string(_Position.z) + " ...");
}

Camera::Camera(float PosX, float PosY, float PosZ, float Yaw, float Pitch, float Roll)
{
	_Position = glm::vec3(PosX, PosY, PosZ);
	_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	_Yaw = Yaw;
	_Pitch = Pitch;
	_Roll = Roll;
	_Zoom = 0;
	_Speed = 0.1f;
	_ConstrainPitch = true;

	UpdateCameraVectors();
	ScreenManager::Instance()->GrabMouse();
	LogManager::Instance()->LogDebug("Creating Camera at: X:" + std::to_string(_Position.x) + " Y:" + std::to_string(_Position.y) + " Z:" + std::to_string(_Position.z) + " ...");
}

Camera::~Camera()
{
	ScreenManager::Instance()->ReleaseMouse();
	LogManager::Instance()->LogDebug("Destroying Camera...");
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(_Position, _Position + _Forward, _Up);
}

glm::vec3 Camera::GetPosition()
{
	return _Position;
}

glm::vec3 Camera::GetDirection()
{
    return _Forward;
}

void Camera::Update()
{
	ProcessKeyEvents();
	ProcessMouseEvents();

	UpdateCameraVectors();
}

void Camera::TogglePitchConstraint()
{
	_ConstrainPitch = !_ConstrainPitch;
}

float Camera::GetZoom() const
{
	return _Zoom;
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(_Yaw)) * cos(glm::radians(_Pitch));
	front.y = sin(glm::radians(_Pitch));
	front.z = sin(glm::radians(_Yaw)) * cos(glm::radians(_Pitch));
	_Forward = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	_Right = glm::normalize(glm::cross(_Forward, _WorldUp));  
	// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	_Up = glm::vec3(0.0, 1.0, 0.0);//glm::normalize(glm::cross(_Right, _Forward));
}

void Camera::ProcessKeyEvents()
{
	if (InputManager::Instance()->IsKeyHeld(SDLK_w)) {
		_Position += _Forward * _Speed;
	}
	if (InputManager::Instance()->IsKeyHeld(SDLK_a)) {
		_Position -= _Right * _Speed;
	}
	if (InputManager::Instance()->IsKeyHeld(SDLK_s)) {
		_Position -= _Forward * _Speed;
	}
	if (InputManager::Instance()->IsKeyHeld(SDLK_d)) {
		_Position += _Right * _Speed;
	}
	if (InputManager::Instance()->IsKeyHeld(SDLK_LSHIFT)) {
		_Position += _Up * _Speed;
	}
	if (InputManager::Instance()->IsKeyHeld(SDLK_LCTRL)) {
		_Position -= _Up * _Speed;
	}
	if (InputManager::Instance()->IsKeyHeld(SDLK_LALT)) {
		InputManager::Instance()->ReleaseMouse();
	}
	else if(InputManager::Instance()->IsKeyReleased(SDLK_LALT)){
		InputManager::Instance()->GrabMouse();
	}
	
}

void Camera::ProcessMouseEvents()
{
	if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
		if (InputManager::Instance()->HasMouseMoved()) {
			_Yaw = glm::mod((float)(_Yaw + InputManager::Instance()->GetMouseMove().xRel), 360.0f);
			_Pitch -= InputManager::Instance()->GetMouseMove().yRel;
		}
		if (_ConstrainPitch)
		{
			if (_Pitch > 89.0f)
				_Pitch = 89.0f;
			if (_Pitch < -89.0f)
				_Pitch = -89.0f;
		}
		if (InputManager::Instance()->HasScrolled()) {
			if (_Zoom >= 1.0f && _Zoom <= 45.0f)
				_Zoom += InputManager::Instance()->GetYScroll();;
			if (_Zoom <= 1.0f)
				_Zoom = 1.0f;
			if (_Zoom >= 45.0f)
				_Zoom = 45.0f;
		}
	}
}
