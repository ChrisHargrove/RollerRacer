#include "Camera.h"
#include "InputManager.h"
#include "ScreenManager.h"
#include <GLM\gtx\rotate_vector.hpp>



Camera::Camera(glm::vec3 position) :
	GameObject(position),
    _Target(nullptr)
{
	_Zoom = 0;
	_Speed = 0.1f;
	_ConstrainPitch = true;

    if ( _Forward.y > 0.98f) {
        _Forward.y = 0.98f;
    }
    if (_Forward.y < -0.98f) {
        _Forward.y = -0.98f;
    }

	ScreenManager::Instance()->GrabMouse();
	InputManager::Instance()->GrabMouse();
}


Camera::~Camera()
{
}

void Camera::Input()
{
	if (InputManager::Instance()->IsKeyHeld(SDLK_w)) {
		_Position += _Forward * _Speed * 10.0f;
	}
	if (InputManager::Instance()->IsKeyHeld(SDLK_a)) {
		_Position -= _Right * _Speed * 10.0f;
	}
	if (InputManager::Instance()->IsKeyHeld(SDLK_s)) {
		_Position -= _Forward * _Speed * 10.0f;
	}
	if (InputManager::Instance()->IsKeyHeld(SDLK_d)) {
		_Position += _Right * _Speed * 10.0f;
	}
	if (InputManager::Instance()->IsKeyHeld(SDLK_LSHIFT)) {
		_Position += _Up * _Speed * 10.0f;
	}
	if (InputManager::Instance()->IsKeyHeld(SDLK_LCTRL)) {
		_Position -= _Up * _Speed * 10.0f;
	}
	if (InputManager::Instance()->IsKeyHeld(SDLK_LALT)) {
		InputManager::Instance()->ReleaseMouse();
	}
	else if (InputManager::Instance()->IsKeyReleased(SDLK_LALT)) {
		InputManager::Instance()->GrabMouse();
	}

	if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
		if (InputManager::Instance()->HasMouseMoved()) {
			_Forward = glm::rotate(_Forward, -(float)InputManager::Instance()->GetMouseMove().xRel * _Speed * 0.1f, _Up);
			_Right = glm::rotate(_Right, -(float)InputManager::Instance()->GetMouseMove().xRel * _Speed * 0.1f, _Up);

			float angle = -(float)InputManager::Instance()->GetMouseMove().yRel * _Speed * 0.1f;
			if (_ConstrainPitch) {
				if (angle > 0 && _Forward.y < 0.98f) {
					_Forward = glm::rotate(_Forward, angle, _Right);
				}
				if(angle < 0 && _Forward.y > -0.98f){
					_Forward = glm::rotate(_Forward, angle, _Right);
				}
			}
			else{
				_Forward = glm::rotate(_Forward, -(float)InputManager::Instance()->GetMouseMove().yRel * _Speed * 0.1f, _Right);
			}
		}
		if (InputManager::Instance()->HasScrolled()) {
			if (_Zoom >= 1.0f && _Zoom <= 45.0f)
				_Zoom += (float)InputManager::Instance()->GetYScroll();
			if (_Zoom <= 1.0f)
				_Zoom = 1.0f;
			if (_Zoom >= 45.0f)
				_Zoom = 45.0f;
		}
	}
}

void Camera::Update()
{
    if (_Target != nullptr) {
		//rotate the camera so that it always faces the target.
		_Forward = _Target->GetRotationQuaternion() * -DEFAULT_FORWARD;
		//keep the camera near the target.
		_Position = _Target->GetPosition() - (_Forward * 7.0f);
		_Position.y += 3.0f;
    }
}

void Camera::Render()
{
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(_Position, _Position + _Forward, _Up);
}

glm::mat4 Camera::GetOrthoView() const
{
	return glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0), _Up);
}

float Camera::GetZoom() const
{
	return _Zoom;
}

void Camera::TogglePitchConstraint()
{
	_ConstrainPitch = !_ConstrainPitch;
}

void Camera::SetTarget(BGameObject * target)
{
    _Target = target;
}
