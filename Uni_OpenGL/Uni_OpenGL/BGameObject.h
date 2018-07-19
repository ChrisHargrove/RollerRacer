#pragma once

#include <vector>
#include <set>
#include <GLM\glm.hpp>
#include <GLM\gtc\quaternion.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM/gtx/vector_angle.hpp>
#include <BULLET\btBulletDynamicsCommon.h>

#include "Model.h"
#include "OpenGLMotionState.h"

#include "MathUtil.h"

class BGameObject
{
public:
    BGameObject(glm::vec3 position, glm::vec3 rotation, float mass, btCollisionShape* shape, std::string shader) {
        _Shader = shader;
        _Scale = glm::vec3(1, 1, 1);
        _Mass = mass;
        
        //store the object shape
        _ObjectShape = shape;
        //create the initial transform
        _Transform.setIdentity();
        _Transform.setOrigin(btVector3(position.x, position.y, position.z));
        _Transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, 1));

    }

    virtual ~BGameObject() {
    }

    virtual void Input() = 0;
    virtual void Update(float delta) = 0;
    virtual void Render(std::string shader = "") = 0;

    virtual void Initialize(btDynamicsWorld* world, std::set<btCollisionShape*>* collisionShapes) {
        //create the motion state
        _MotionState = new OpenGLMotionState(_Transform);
        //calculate local inertia
        btVector3 localInertia(0, 0, 0);
        //infinite mass cant move or rotate: 0 = inifinite
        if (_Mass != 0.0f) {
            _ObjectShape->calculateLocalInertia(_Mass, localInertia);
        }
        //create a rigid body for the object
        btRigidBody::btRigidBodyConstructionInfo bodyInfo(_Mass, _MotionState, _ObjectShape, localInertia);
        //create the body
        _ObjectBody = new btRigidBody(bodyInfo);

		collisionShapes->emplace(_ObjectShape);
        world->addRigidBody(_ObjectBody);
    }

	btCollisionShape* GetShape() { return _ObjectShape; }
	btRigidBody* GetRigidBody() { return _ObjectBody; }
	btMotionState* GetMotionState() { return _MotionState; }

	glm::mat4 GetModelMatrix() {
		if (_MotionState) { return glm::scale(_MotionState->GetWorldMatrix(), _Scale); }
        else { return glm::mat4(1.0f); }
	}

    glm::vec3 GetPosition() {
		_MotionState->getWorldTransform(_Transform);

		btVector3 temp = _Transform.getOrigin();//_ObjectBody->getWorldTransform().getOrigin();
        return glm::vec3(temp.x(), temp.y(), temp.z());
    }

    glm::vec3 GetRotation() {
        btQuaternion temp = _Transform.getRotation();
        return glm::vec3(temp.x(), temp.y(), temp.z());
    }

    glm::quat GetRotationQuaternion() {
		_MotionState->getWorldTransform(_Transform);//_ObjectBody->getWorldTransform().getRotation();
		btQuaternion temp = _Transform.getRotation();
        return glm::quat(temp.w(), temp.x(), temp.y(), temp.z());
    }

	void Rotate(glm::vec3 eularAngle) {
		_MotionState->getWorldTransform(_Transform);
		btQuaternion originRot = _Transform.getRotation();
		originRot *=  glmQuatToBullet(glm::quat(eularAngle));
		_Transform.setRotation(originRot);
		_ObjectBody->setWorldTransform(_Transform);
	}

	void SetDirection(glm::vec3 direction) {
		btVector3 defaultForward(0,0,1);

		btQuaternion rot = fromVecToVec(defaultForward, glmVecToBullet(direction));
		_MotionState->getWorldTransform(_Transform);
		_Transform.setRotation(rot);
		_ObjectBody->setWorldTransform(_Transform);

	}

    void SetPosition(glm::vec3 position) {
		_MotionState->getWorldTransform(_Transform);
        _Transform.setOrigin(btVector3(position.x, position.y, position.z));
        _ObjectBody->setWorldTransform(_Transform);
		_MotionState->setWorldTransform(_Transform);
    }
    void SetRotation(glm::vec3 rotation) {
        _Transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, 1));
        _ObjectBody->setWorldTransform(_Transform);
    }
    void SetLinearVelocity(glm::vec3 velocity) {
        _ObjectBody->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity .x));
    }
    void SetAngularVelocity(glm::vec3 velocity) {
        _ObjectBody->setAngularVelocity(btVector3(velocity.x, velocity.y, velocity.x));
    }

    std::string GetShader() {
        return _Shader;
    }

    void Scale(glm::vec3 scale) {
        //_ObjectShape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
        _Scale = scale;
    }

    void SetActivationState(bool alwaysActive = false) {
        if (alwaysActive) {
            _ObjectBody->setActivationState(DISABLE_DEACTIVATION);
        }
        else {
            _ObjectBody->setActivationState(ACTIVE_TAG);
        }
    }

protected:
	btCollisionShape*	_ObjectShape;
	btRigidBody*		_ObjectBody;
	OpenGLMotionState*	_MotionState;
    btTransform         _Transform;
    glm::vec3           _Scale;

    float _Mass;

    std::string _Shader;
};