#pragma once

#include "BGameObject.h"
#include <functional>
#include <GLM/glm.hpp>

class BTriggerVolume : public BGameObject
{
public:
	BTriggerVolume(glm::vec3 position, glm::vec3 rotation, btCollisionShape* shape, std::string shader);
	virtual ~BTriggerVolume();

	virtual void Initialize(btDynamicsWorld* world, std::set<btCollisionShape*>* collisionShapes) override;

	void SetTrackDirecion(glm::vec3 direction);
	glm::vec3 GetTrackDirection() { return _DirectionOfTrack; }
	float GetAngle() { return _AngleFromZ; }

	void SetCallBack(std::function<void()> callback);
	void SetAltCallBack(std::function<void()> callback);
	void CallBack();
	void AltCallBack();
private:

	std::function<void()> _CallBack;
	std::function<void()> _AltCallBack;

	// Inherited via BGameObject
	virtual void Input() override;
	virtual void Update(float delta) override;
	virtual void Render(std::string shader = "") override;

	glm::vec3 _DirectionOfTrack;
	float _AngleFromZ;
};

