#include "PhysicsManager.h"
#include "InputManager.h"
#include <SDL\SDL.h>

#include "BGameObject.h"
#include "Terrain.h"
#include "Level.h"
#include <iterator>

PhysicsManager PhysicsManager::_Instance;

PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::Initialize()
{
	//create all physics requirements for the bullet library
	_CollisionConfig = new btDefaultCollisionConfiguration();
	_Dispatcher = new btCollisionDispatcher(_CollisionConfig);
	_BroadPhaseDetection = new btDbvtBroadphase();
	_Solver = new btSequentialImpulseConstraintSolver();
	_World = new btDiscreteDynamicsWorld(_Dispatcher, _BroadPhaseDetection, _Solver, _CollisionConfig);
	_World->setGravity(btVector3(0, -9.8f, 0));

	_DebugDrawer = new OpenGLDebugDrawer();
	_DebugDrawer->setDebugMode(0);
	_World->setDebugDrawer(_DebugDrawer);
}

void PhysicsManager::Shutdown()
{
	//Start by remooving all rigid bodies from world
	for (int i = _World->getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject* obj = _World->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}
		_World->removeCollisionObject(obj);
		delete obj;
	}
	//Next Delete all collision shapes
	for (auto c : _CollisionShapes) {
		delete c;
	}
	_CollisionShapes.clear();

	delete _World;
	delete _Solver;
	delete _BroadPhaseDetection;
	delete _Dispatcher;
	delete _CollisionConfig;
	delete _DebugDrawer;

}

void PhysicsManager::Input()
{
	//does all debug drawing. commented so not available in final game.
	/*if (InputManager::Instance()->IsKeyPressed(SDLK_F1)) {
		_DebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawWireframe);
	}
	if (InputManager::Instance()->IsKeyPressed(SDLK_F2)) {
		_DebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawAabb);
	}
	if (InputManager::Instance()->IsKeyPressed(SDLK_F3)) {
		_DebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawContactPoints);
	}
	if (InputManager::Instance()->IsKeyPressed(SDLK_F4)) {
		_DebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawNormals);
	}
	if (InputManager::Instance()->IsKeyPressed(SDLK_F5)) {
		_DebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawConstraints);
	}*/

}

void PhysicsManager::Update(float delta)
{
	//run 1 frame of physics whihc updates 60 times a frame
	//at 60fps minimum, so thats 3600 physics updates per second.
	_World->stepSimulation(delta);
	CheckForCollisionEvents();
}

void PhysicsManager::Render()
{
	//purely for drawing the debug boxes around objects.
	_World->debugDrawWorld();
	_DebugDrawer->Render();
}

void PhysicsManager::AddObjectToSimulation(BGameObject * object)
{
	object->Initialize(_World, &_CollisionShapes);
}

void PhysicsManager::AddObjectToSimulation(Terrain * terrain)
{
	terrain->Initialize(_World, &_CollisionShapes);
}

void PhysicsManager::AddLevelToSimulation(Level * level)
{
	level->Initialize(_World, &_CollisionShapes);
}

void PhysicsManager::RemoveObjectFromSimulation(BGameObject * object)
{
	_World->removeRigidBody(object->GetRigidBody());
}

void PhysicsManager::RemoveObjectFromSimulation(Terrain * terrain)
{
	_World->removeRigidBody(terrain->GetRigidBody());
}

void PhysicsManager::RemoveLevelFromSimulation(Level * level)
{
	_World->removeRigidBody(level->GetTerrain()->GetRigidBody());
}

bool PhysicsManager::CheckCollision(BGameObject * objectOne, BGameObject * objectTwo)
{
	if (_CurrentlyCollidingPairs.find(std::make_pair(objectOne->GetRigidBody(), objectTwo->GetRigidBody())) != _CurrentlyCollidingPairs.end()) {
		return true;
	}
	if (_CurrentlyCollidingPairs.find(std::make_pair(objectTwo->GetRigidBody(), objectOne->GetRigidBody())) != _CurrentlyCollidingPairs.end()) {
		return true;
	}
	return false;
}

bool PhysicsManager::CheckSingleCollision(BGameObject * objectOne, BGameObject * objectTwo)
{
	if (_CurrentlyCollidingPairs.find(std::make_pair(objectOne->GetRigidBody(), objectTwo->GetRigidBody())) != _CurrentlyCollidingPairs.end()) {
		_CurrentlyCollidingPairs.erase(std::make_pair(objectOne->GetRigidBody(), objectTwo->GetRigidBody()));
		return true;
	}
	if (_CurrentlyCollidingPairs.find(std::make_pair(objectTwo->GetRigidBody(), objectOne->GetRigidBody())) != _CurrentlyCollidingPairs.end()) {
		_CurrentlyCollidingPairs.erase(std::make_pair(objectTwo->GetRigidBody(), objectOne->GetRigidBody()));
		return true;
	}
	return false;
}

bool PhysicsManager::CheckSingleExit(BGameObject * objectOne, BGameObject * objectTwo)
{
	if (_CurrentlyCollidingPairs.find(std::make_pair(objectOne->GetRigidBody(), objectTwo->GetRigidBody())) == _CurrentlyCollidingPairs.end() &&
		_PreviousCollisionPairs.find(std::make_pair(objectOne->GetRigidBody(), objectTwo->GetRigidBody())) != _PreviousCollisionPairs.end()) {
		return true;
	}
	if (_CurrentlyCollidingPairs.find(std::make_pair(objectTwo->GetRigidBody(), objectOne->GetRigidBody())) == _CurrentlyCollidingPairs.end() &&
		_PreviousCollisionPairs.find(std::make_pair(objectTwo->GetRigidBody(), objectOne->GetRigidBody())) != _PreviousCollisionPairs.end()) {
		return true;
	}
	return false;
}

void PhysicsManager::CheckForCollisionEvents()
{
	//Keep a list of collision pairs we found in the current update
	std::set<std::pair<const btRigidBody*, const btRigidBody*>> _CurrentCollisionPairs;
	//loop through all manifolds in the dispatcher.
	for (int i = 0; i < _Dispatcher->getNumManifolds(); i++) {
		//get the current manifold
		btPersistentManifold* _Manifold = _Dispatcher->getManifoldByIndexInternal(i);
		//ignore manifolds with no contact points
		if (_Manifold->getNumContacts() > 0) {
			//get the rigid bodies involved in the collision
			const btRigidBody* _BodyOne = static_cast<const btRigidBody*>(_Manifold->getBody0());
			const btRigidBody* _BodyTwo = static_cast<const btRigidBody*>(_Manifold->getBody1());

			//always make sure the pairs are in memory order so allow for comparing pairs
			bool needSwapping = _BodyOne > _BodyTwo;
			const btRigidBody* _SortedBodyOne = needSwapping ? _BodyTwo : _BodyOne;
			const btRigidBody* _SortedBodyTwo = needSwapping ? _BodyOne : _BodyTwo;

			//create the pair
			std::pair<const btRigidBody*, const btRigidBody*> _CurrentPair = std::make_pair(_SortedBodyOne, _SortedBodyTwo);
			//insert the pair into the current list
			_CurrentCollisionPairs.insert(_CurrentPair);

			//if pair isnt in the previous pair list send a collision event.
			if (_PreviousCollisionPairs.find(_CurrentPair) == _PreviousCollisionPairs.end()) {
				CollisionEvent(_CurrentPair);
			}
		}
	}

	//Create another list for any pairs that were removed this update
	std::set<std::pair<const btRigidBody*, const btRigidBody*>> _RemovedCollisionPairs;
	//std set allows you to efficiently get the difference between 2 sets
	std::set_difference(_PreviousCollisionPairs.begin(), _PreviousCollisionPairs.end(),
		_CurrentCollisionPairs.begin(), _CurrentCollisionPairs.end(),
		std::inserter(_RemovedCollisionPairs, _RemovedCollisionPairs.begin()));
	//loop through all removed pairs
	for (auto p : _RemovedCollisionPairs) {
		SeperationEvent(std::make_pair((btRigidBody*)p.first, (btRigidBody*)p.second));
	}

	//set the previous collision pair list equal to this updates pair list.
	_PreviousCollisionPairs = _CurrentCollisionPairs;
}

void PhysicsManager::CollisionEvent(std::pair<const btRigidBody*, const btRigidBody*> pair)
{
	_CurrentlyCollidingPairs.insert(pair);
}

void PhysicsManager::SeperationEvent(std::pair<const btRigidBody*, const btRigidBody*> pair)
{
	_CurrentlyCollidingPairs.erase(pair);
}

btBroadphaseInterface * PhysicsManager::GetBroadPhaseInterface()
{
	return _BroadPhaseDetection;
}

btCollisionConfiguration * PhysicsManager::GetCollisionConfiguration()
{
	return _CollisionConfig;
}

btCollisionDispatcher * PhysicsManager::GetCollisionDispatcher()
{
	return _Dispatcher;
}

btConstraintSolver * PhysicsManager::GetConstraintSolver()
{
	return _Solver;
}

btDynamicsWorld * PhysicsManager::GetWorld()
{
	return _World;
}

OpenGLDebugDrawer * PhysicsManager::GetDebugDrawer()
{
	return _DebugDrawer;
}
