#pragma once

#include "GameObject.h"
#include <GLM\glm.hpp>

#include "BGameObject.h"

class Camera : public GameObject
{
public:
	Camera(glm::vec3 position);
	virtual ~Camera();

	// Inherited via GameObject
	virtual void Input() override;
	virtual void Update() override;
	virtual void Render() override;

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetOrthoView() const;

	float GetZoom() const;

	void TogglePitchConstraint();

    void SetTarget(BGameObject* target);

private:
	float _Zoom;
	float _Speed;
	bool _ConstrainPitch;

    BGameObject* _Target;
};

