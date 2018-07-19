#pragma once

#include <set>
#include <vector>
#include <deque>
#include <BULLET\btBulletDynamicsCommon.h>

#include "Timer.h"
#include "Terrain.h"
#include "BTriggerVolume.h"
#include "TrackGenerator.h"
#include "CatmullRomSpline.h"

class Texture;
class Model;
class BModel;

class Level
{
public:
	Level(int seed, std::vector<Texture*> texList, std::vector<Model*> foliageList);
	~Level();

	void CreateLevel();

	void Initialize(btDynamicsWorld* world, std::set<btCollisionShape*>* collisionShapes);
	void StartLapTimer() { _LapTimer->Start(); }
	
	Terrain* GetTerrain() { return _LevelTerrain; }
	TrackGenerator* GetTrack() { return _LevelTrack; }
	CatmullRomSpline* GetSmoothTrack() { return _LevelTrackSmoother; }

	float GetCurrentLapTime() { return _LapTimer->GetElapsedTime(); }
	float GetBestLapTime();
	BTriggerVolume* GetNextTriggerGate() { return _TriggerGates.front(); }

	int GetNearestTrackPoint(glm::vec3 from);

	void Render(std::string shader = "");
	void Update(float delta);

	StopWatch* GetStopWatch() { return _LapTimer; }

private:
	int _Seed;

	StopWatch* _LapTimer;
	float _LapTime = 0.0f;

	btBoxShape* _TriggerGateShape;
	std::deque<BTriggerVolume*> _TriggerGates;
	std::deque<float> _LapTimes;

	Terrain* _LevelTerrain;
	TrackGenerator* _LevelTrack;
	CatmullRomSpline* _LevelTrackSmoother;

	std::vector<Model*> _FoliageModelList;
	std::vector<BModel*> _FoliageBModelList;

};

