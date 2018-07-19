#pragma once

#include "BGameObject.h"
#include "Model.h"
#include <string>

class BVehicle : public BGameObject
{
public:
    BVehicle(glm::vec3 position, glm::vec3 rotation, float mass, Model* model, std::string shader);
    virtual ~BVehicle();

    virtual void Initialize(btDynamicsWorld* world, std::set<btCollisionShape*>* collisionShapes) override;

	float GetCurrentSpeedKmHour();

    // Inherited via BGameObject
    virtual void Input() override;
    virtual void Update(float delta) override;
    virtual void Render(std::string shader = "") override;

    void Scale(glm::vec3 scale);

private:
    Model * _Model;

    btVehicleRaycaster * _VehicleRayCaster;
    btRaycastVehicle*   _Vehicle;

    float _Steering = 0.0f;
    float _SteeringIncrement = 0.03f;
    float _MaxSteering = 0.3f;

    float _EngineForce = 0.0f;
    float _AccelerationRate = 200.0f;
    float _MaxEngineForce = 3000.0f;
	float _MaxReverseEngineForce = 1500.0f;
	float _MaxSpeed = 180.0f;

    float _BrakeForce = 0.0f;
    float _MaxBrakeForce = 100.0f;

	bool _Reversing = false;

    void AddWheels(btVector3* halfSize, btRaycastVehicle* vehicle, btRaycastVehicle::btVehicleTuning tuning);
};

