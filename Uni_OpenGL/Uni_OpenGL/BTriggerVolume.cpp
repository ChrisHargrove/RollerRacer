#include "BTriggerVolume.h"
#include <glm/gtx/vector_angle.hpp>



BTriggerVolume::BTriggerVolume(glm::vec3 position, glm::vec3 rotation, btCollisionShape* shape, std::string shader) :
	BGameObject(position, rotation, 0.0f, shape, shader)
{
}


BTriggerVolume::~BTriggerVolume()
{
	BGameObject::~BGameObject();
}

void BTriggerVolume::Initialize(btDynamicsWorld * world, std::set<btCollisionShape*>* collisionShapes)
{
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
	_ObjectBody->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

	collisionShapes->emplace(_ObjectShape);
	world->addRigidBody(_ObjectBody);
}

void BTriggerVolume::SetTrackDirecion(glm::vec3 direction)
{
	_DirectionOfTrack = direction;
	_AngleFromZ = glm::angle(glm::vec3(0, 0, 1), direction);
}

void BTriggerVolume::SetCallBack(std::function<void()> callback)
{
	_CallBack = callback;
}

void BTriggerVolume::SetAltCallBack(std::function<void()> callback)
{
	_AltCallBack = callback;
}

void BTriggerVolume::CallBack()
{
	_CallBack();
}

void BTriggerVolume::AltCallBack()
{
	_AltCallBack();
}

void BTriggerVolume::Input()
{
}

void BTriggerVolume::Update(float delta)
{
}

void BTriggerVolume::Render(std::string shader)
{
}
