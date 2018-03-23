#ifndef CAMERA_H
#define CAMERA_H

#include <GLM\glm.hpp>

class Camera
{
public:
	Camera(glm::vec3 Position = glm::vec3(0.0f,0.0f,0.0f), float Yaw = 0.0f, float Pitch = 0.0f, float Roll = 0.0f);
	Camera(float PosX, float PosY, float PosZ, float Yaw = 0.0f, float Pitch = 0.0f, float Roll = 0.0f);
	~Camera();

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetOrthoView() const;
	glm::vec3 GetPosition();
    glm::vec3 GetDirection();
	void Update();
	void TogglePitchConstraint();
	float GetZoom() const;

private:
	glm::vec3 _Position;
	glm::vec3 _Forward;
	glm::vec3 _WorldUp;
	glm::vec3 _Up;
	glm::vec3 _Right;

	float _Yaw;
	float _Pitch;
	float _Roll;
	float _Zoom;
	float _Speed;

	bool _ConstrainPitch;

	void UpdateCameraVectors();
	void ProcessKeyEvents();
	void ProcessMouseEvents();
};

#endif