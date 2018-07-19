#pragma once

#include <BULLET\btBulletCollisionCommon.h>
#include <BULLET\btBulletDynamicsCommon.h>
#include "OpenGLMotionState.h"
#include "OpenGLDebugDrawer.h"

#include <set>

class BGameObject;
class Terrain;
class Level;

class PhysicsManager
{
public:
	void Initialize();
	void Shutdown();

	void Input();
	void Update(float delta);
	void Render();

	void AddObjectToSimulation(BGameObject* object);
	void AddObjectToSimulation(Terrain* terrain);
	void AddLevelToSimulation(Level* level);

	void RemoveObjectFromSimulation(BGameObject* object);
	void RemoveObjectFromSimulation(Terrain* terrain);
	void RemoveLevelFromSimulation(Level* level);

	bool CheckCollision(BGameObject* objectOne, BGameObject* objectTwo);
	bool CheckSingleCollision(BGameObject * objectOne, BGameObject * objectTwo);
	bool CheckSingleExit(BGameObject * objectOne, BGameObject * objectTwo);

	btBroadphaseInterface* GetBroadPhaseInterface();
	btCollisionConfiguration* GetCollisionConfiguration();
	btCollisionDispatcher* GetCollisionDispatcher();
	btConstraintSolver* GetConstraintSolver();
	btDynamicsWorld* GetWorld();
	OpenGLDebugDrawer* GetDebugDrawer();

	static PhysicsManager* Instance() {
		return &_Instance;
	}

private:
	static PhysicsManager _Instance;

	btBroadphaseInterface*          _BroadPhaseDetection;
	btCollisionConfiguration*       _CollisionConfig;
	btCollisionDispatcher*          _Dispatcher;
	btConstraintSolver*             _Solver;
	btDynamicsWorld*                _World;
	OpenGLDebugDrawer*              _DebugDrawer;

	std::set<std::pair<const btRigidBody*, const btRigidBody*>> _PreviousCollisionPairs;

	std::set<std::pair<const btRigidBody*, const btRigidBody*>> _CurrentlyCollidingPairs;

	void CheckForCollisionEvents();
	void CollisionEvent(std::pair<const btRigidBody*, const btRigidBody*> pair);
	void SeperationEvent(std::pair<const btRigidBody*, const btRigidBody*> pair);

	std::set<btCollisionShape*> _CollisionShapes;

	PhysicsManager();
	~PhysicsManager();
	PhysicsManager(const PhysicsManager&) {}
};

