#include "BVehicle.h"
#include "RigidBodyFromOBJ.h"
#include <SDL/sdl.h>
#include "InputManager.h"
#include "PhysicsManager.h"

#include <iostream>

BVehicle::BVehicle(glm::vec3 position, glm::vec3 rotation, float mass, Model* model, std::string shader) :
    BGameObject(position, rotation, mass, new btBoxShape(btVector3(1.7272f, 1.2192f, 3.3528f) * 0.5f), shader)
{
    _Model = model;
}

BVehicle::~BVehicle()
{
}

void BVehicle::Initialize(btDynamicsWorld * world, std::set<btCollisionShape*>* collisionShapes)
{
	//create a transform to start the initialization.
	btTransform localTransform;
	localTransform.setIdentity();
	localTransform.setOrigin(btVector3(0.0f, 0.4f, 0.0f));

	//use a compoound shape to set the transform at 0,0,0
	btCompoundShape* compound = new btCompoundShape();
	//by adding the object shape to a compound shape to effectively
	//shift the center of gravity 
	compound->addChildShape(localTransform, _ObjectShape);

    //calculate local inertia
    btVector3 localInertia(0, 0, 0);
    //infinite mass cant move or rotate: 0 = inifinite
    if (_Mass != 0.0f) {
        _ObjectShape->calculateLocalInertia(_Mass, localInertia);
    }
	_MotionState = new OpenGLMotionState(_Transform);
    //create a rigid body for the object
    btRigidBody::btRigidBodyConstructionInfo bodyInfo(_Mass, _MotionState, compound, localInertia);
    //create the body
    _ObjectBody = new btRigidBody(bodyInfo);

	collisionShapes->emplace(compound);
	collisionShapes->emplace(_ObjectShape);
    world->addRigidBody(_ObjectBody);

    _VehicleRayCaster = new btDefaultVehicleRaycaster(world);
    btRaycastVehicle::btVehicleTuning _VehicleTuning;
    _Vehicle = new btRaycastVehicle(_VehicleTuning, _ObjectBody, _VehicleRayCaster);
    _ObjectBody->setActivationState(DISABLE_DEACTIVATION);
	//_ObjectBody->setFriction(btScalar(0.6));

    world->addVehicle(_Vehicle);
	btVector3 bodySize = btVector3(1.7272f, 1.2192f, 3.3528f) * 0.5f;
    AddWheels(&bodySize, _Vehicle, _VehicleTuning);
}

float BVehicle::GetCurrentSpeedKmHour()
{
	return abs(_Vehicle->getCurrentSpeedKmHour());
}

void BVehicle::AddWheels(btVector3* halfSize, btRaycastVehicle * vehicle, btRaycastVehicle::btVehicleTuning tuning)
{
    btVector3 _WheelDirection(0, -1, 0);
    btVector3 _WheelAxis(-1, 0, 0);

    btScalar _SuspensionRestLength(0.4f);

    btScalar _WheelWidth(0.3f);
    btScalar _WheelRadius(0.7f);

	vehicle->addWheel(btVector3(-halfSize->x(), 0.45f, halfSize->z()), _WheelDirection, _WheelAxis, _SuspensionRestLength, _WheelRadius, tuning, true);
	vehicle->addWheel(btVector3(halfSize->x(), 0.45f, halfSize->z()), _WheelDirection, _WheelAxis, _SuspensionRestLength, _WheelRadius, tuning, true);
	vehicle->addWheel(btVector3(-halfSize->x(), 0.45f, -halfSize->z()), _WheelDirection, _WheelAxis, _SuspensionRestLength, _WheelRadius, tuning, false);
	vehicle->addWheel(btVector3(halfSize->x(), 0.45f, -halfSize->z()), _WheelDirection, _WheelAxis, _SuspensionRestLength, _WheelRadius, tuning, false);


    //configure each wheel
    for (int i = 0; i < vehicle->getNumWheels(); i++) {
        btWheelInfo& wheel = vehicle->getWheelInfo(i);
        wheel.m_suspensionStiffness = 15.0f;
		wheel.m_wheelsDampingRelaxation = 2.3f;
		wheel.m_wheelsDampingCompression = 4.4f;
		//wheel.m_maxSuspensionTravelCm = 500.0f;
		//wheel.m_maxSuspensionForce = 5000000.0f;
		//if (i > 1) {
		//	wheel.m_frictionSlip = 1000;
		//}
		//else {
		//	wheel.m_frictionSlip = 1000;
		//}

        //smaller roll influence reduces vehicle roll
        wheel.m_rollInfluence = 0.1f;
        wheel.m_steering = 0.0f;
    }
}

void BVehicle::Input()
{
    if (InputManager::Instance()->IsKeyHeld(SDLK_LEFT)) {
        _Steering += 0.03f;
        if (_Steering > _MaxSteering) {
            _Steering = _MaxSteering;
        }
    }
    if (InputManager::Instance()->IsKeyHeld(SDLK_RIGHT)) {
        _Steering -= 0.03f;
        if (_Steering < -_MaxSteering) {
            _Steering = -_MaxSteering;
        }
    }

    if (!InputManager::Instance()->IsKeyHeld(SDLK_LEFT) && !InputManager::Instance()->IsKeyHeld(SDLK_RIGHT)) {
        _Steering = 0.0f;
    }

    if (InputManager::Instance()->IsKeyHeld(SDLK_UP) && !InputManager::Instance()->IsKeyHeld(SDLK_DOWN)) {
		if (GetCurrentSpeedKmHour() < _MaxSpeed) {
			_EngineForce += _AccelerationRate;
		}
        if (_EngineForce > _MaxEngineForce) {
            _EngineForce = _MaxEngineForce;
        }
        _BrakeForce = 0.0f;
		_Reversing = false;
    }

	if(!InputManager::Instance()->IsKeyHeld(SDLK_UP) && !InputManager::Instance()->IsKeyHeld(SDLK_DOWN)) {
		_EngineForce = 0.0f;
		_BrakeForce = 1.0f;
		_Reversing = false;
	}

    if (InputManager::Instance()->IsKeyHeld(SDLK_DOWN)) {
		if (abs(_Vehicle->getCurrentSpeedKmHour()) <=1 &&  !_Reversing) {
			_Reversing = true;
		}
		if (!_Reversing) {
			_BrakeForce = _MaxBrakeForce;
			if (_BrakeForce > _MaxBrakeForce) {
				_BrakeForce = _MaxBrakeForce;
			}
			_EngineForce = 0.0f;
		}
		else{
			_BrakeForce = 0.0f;
			if (GetCurrentSpeedKmHour() < _MaxSpeed) {
				_EngineForce -= _AccelerationRate;
			}
			if (_EngineForce < -_MaxReverseEngineForce) {
				_EngineForce = -_MaxReverseEngineForce;
			}
		}
    }
}

void BVehicle::Update(float delta)
{
    _Vehicle->setSteeringValue(_Steering, 0);
    _Vehicle->setSteeringValue(_Steering, 1);

	//testing better steering -- give better steering qwhen under power
	//but otherwise makes the vehicle very unstable
	//_Vehicle->setSteeringValue(-_Steering, 2);
	//_Vehicle->setSteeringValue(-_Steering, 3);

    _Vehicle->applyEngineForce(_EngineForce, 2);
    _Vehicle->applyEngineForce(_EngineForce, 3);
	_Vehicle->setBrake(_BrakeForce, 0);
	_Vehicle->setBrake(_BrakeForce, 1);
    _Vehicle->setBrake(_BrakeForce, 2);
    _Vehicle->setBrake(_BrakeForce, 3);

	//std::cout << "Car Speed Km/H: " << abs(_Vehicle->getCurrentSpeedKmHour()) << std::endl;
}

void BVehicle::Render(std::string shader)
{
    _Model->Render(shader);
}

void BVehicle::Scale(glm::vec3 scale)
{
    BGameObject::Scale(scale);
}
