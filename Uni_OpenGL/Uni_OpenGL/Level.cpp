#include "Level.h"
#include "MathUtil.h"
#include "Model.h"
#include "BModel.h"
#include "PRNG.h"

#include "ShaderManager.h"

#include <iostream>

Level::Level(int seed, std::vector<Texture*> texList, std::vector<Model*> foliageList)
{
	_Seed = seed;
	_LevelTerrain = new Terrain(0, 0, texList, nullptr);
	_LevelTrack = new TrackGenerator();
	_LevelTrackSmoother = new CatmullRomSpline();
	_FoliageModelList = foliageList;
	_LapTime = 0.0f;
	_LapTimes.clear();
	_LapTimer = new StopWatch();
}


Level::~Level()
{
	//clear everything :)  no leaks here :D
	delete _LevelTerrain;
	delete _LevelTrack;
	delete _LevelTrackSmoother;
	delete _LapTimer;
	for (auto t : _TriggerGates) {
		delete t;
	}
}

void Level::CreateLevel()
{
	//create terrain plane
	_LevelTerrain->SetSeed(_Seed);
	_LevelTerrain->GenerateTerrain();
	_LevelTerrain->FormatVertexData();

	//create initial track
	_LevelTrack->SetSeed(_Seed);
	_LevelTrack->GeneratePoints();

	//smooth the track out so corners arent diabolical
	_LevelTrackSmoother->SetOriginalPoints(*_LevelTrack->GetPoints());
	_LevelTrackSmoother->SetCurvePointCount(50.0f);
	_LevelTrackSmoother->GenerateSpline();

	//Add spline points to terrain, to change texture data
	_LevelTerrain->AddSplineToTerrain(_LevelTrackSmoother);
	_LevelTerrain->CalculateNormals();

	//Add terrain vertices to the GPU via VBO's
	_LevelTerrain->AddToBuffer();
	
	//Add Trigger boxes to each major track point
	//will be used to make sure player is folling track. and add track segment times.
	_TriggerGateShape = new btBoxShape(btVector3(15, 4, 15));
	std::vector<glm::vec2> _TrackPoints = *_LevelTrack->GetPoints();
	std::vector<glm::vec2> _SplinePoints = _LevelTrackSmoother->GetSpline();
	int curvePointCount = _LevelTrackSmoother->GetCurvePointCount();

	//for each track point add a trigger volume
	for (int i = 0; i < (int)_TrackPoints.size(); i++) {
		glm::vec2 point = _TrackPoints[i] + glm::vec2(_LevelTerrain->GetSize() * 0.5f);
		glm::vec2 next; //find the next point on the spline
		next = _SplinePoints[(i + 1) * curvePointCount];

		//adjust the track point so its aigned with the terrain vertices.
		glm::vec2 np = glm::vec2(point / _LevelTerrain->GetVertexSpacing());
		np.x = roundf(np.x); //round the point to integers so its direcly above a terrain vertex.
		np.y = roundf(np.y);

		//offset the point so its centered to the terrain.
		point -= glm::vec2(_LevelTerrain->GetSize() * 0.5f);

		//calculate the position of the trigger volume
		glm::vec3 gatePos = glm::vec3(point.x, _LevelTerrain->GetHeight((int)np.x, (int)np.y), point.y);

		//create the new trigger volume
		BTriggerVolume* newGate = new BTriggerVolume(gatePos, glm::vec3(0,1,0), _TriggerGateShape, "");
		//calculate the direction the trigger volume is facing, so it faces the direction of the track
		glm::vec3 direction = (glm::vec3(next.x, 0, next.y) - glm::vec3(point.x, 0, point.y));
		newGate->SetTrackDirecion(glm::normalize(direction));

		//setting the callback funtions of the trigger volumes using lambda functions and the std::functional header.
		if (i < 1) { //give the first trigger volume a unique callback so that is saves the completed lap time
			newGate->SetCallBack([this]() { 
				if (!_LapTimer->IsTiming()) {
					StartLapTimer();
					_TriggerGates.push_back(_TriggerGates.front());
					_TriggerGates.pop_front();
				}
				else {
					_LapTimes.push_back(_LapTimer->GetElapsedTime());
					_LapTimer->Reset();
					_TriggerGates.push_back(_TriggerGates.front());
					_TriggerGates.pop_front();
				}
			});
		}
		else { //all other trigger volumes only calculate the segment time.
			newGate->SetCallBack([this]() {
				_TriggerGates.push_back(_TriggerGates.front());
				_TriggerGates.pop_front();
			});
		}
		//add each trigger volume to the vector for later initialization.
		_TriggerGates.push_back(newGate);
	}

	//add all the terrain objects like rocks and cacti, but make sure they are not too close to the track
	PRNG _NumberGenerator(_Seed, 60, 100); //Random Number of foliage
	PRNG _ModelChooser(_Seed,0,(int)_FoliageModelList.size()); //used to choose the model
	int foliageCount = (int)_NumberGenerator.GetNumber();
	_NumberGenerator.SetRange(1, _LevelTerrain->GetVertexCount() - 1); //reset range for generating position.
	for (int i = 0; i < foliageCount; i++) {
		//Get the position of the model
		glm::vec2 tempPos = glm::vec2(_NumberGenerator.GetNumberF(), _NumberGenerator.GetNumberF());
		tempPos.x = roundf(tempPos.x);
		tempPos.y = roundf(tempPos.y);
		//create a 3d position with the height of terrain vertex
		//add a small offset vertically so that it doesnt fall throught the floor.
		glm::vec3 pos = glm::vec3(tempPos.x * _LevelTerrain->GetVertexSpacing(), _LevelTerrain->GetHeight((int)tempPos.x, (int)tempPos.y) + 0.1f, tempPos.y * _LevelTerrain->GetVertexSpacing());
		pos -= glm::vec3(_LevelTerrain->GetSize() * 0.5f, 0, _LevelTerrain->GetSize() * 0.5f);
		//use the new position and get a random model for the foliage.
		BModel* model = new BModel(pos, glm::vec3(0, 1, 0), 50.0f, _FoliageModelList[(int)_ModelChooser.GetNumberF()], "betterLight");
		_FoliageBModelList.push_back(model);
	}
}

void Level::Initialize(btDynamicsWorld* world, std::set<btCollisionShape*>* collisionShapes)
{
	//initialize each component of the level so that they are now in the 
	//physics simulation.
	_LevelTerrain->Initialize(world, collisionShapes);
	//modifying the friction so vehicles get better traction
	//not actually sure if this is working or not. :/
	_LevelTerrain->GetRigidBody()->setFriction(btScalar(1));
	for (auto t : _TriggerGates) {
		//initialize each trigger volume.
		t->Initialize(world, collisionShapes);
		t->SetDirection(t->GetTrackDirection());
	}
	for (auto f : _FoliageBModelList) {
		//initialize each piece of foliage.
		f->Initialize(world, collisionShapes);
	}
}

float Level::GetBestLapTime()
{
	//sort all the laptimes so that you always get the best one
	if (_LapTimes.size() > 0) {
		std::sort(_LapTimes.begin(), _LapTimes.end());
		if (_LapTimes.front() > 0.0f) {
			return _LapTimes.front();
		}
		else {
			_LapTimes.pop_front();
			return _LapTimes.front();
		}
	}
	else {
		//if no laptimes yet then just return 0.
		return 0.0f;
	}
}

int Level::GetNearestTrackPoint(glm::vec3 from)
{
	//get all spline points
	std::vector<glm::vec2> _TrackPoints = _LevelTrackSmoother->GetSpline();
	int currentClosestIndex = 0; //we need the index for later use.

	//set the first point in the vector as the shortest distance
	float distanceFrom = glm::length(glm::vec3(_TrackPoints[0].x, 0.0f, _TrackPoints[0].y) - from);
	for (int i = 0; i < (int)_TrackPoints.size(); i++) {
		//calculate the distance from the supplied vec3 t the point and check if it is closer or not
		float tempDistance = glm::length(glm::vec3(_TrackPoints[i].x, 0.0f, _TrackPoints[i].y) - from);
		if (tempDistance < distanceFrom) {
			//if close save this index as the current closest and change the distance.
			currentClosestIndex = i;
			distanceFrom = tempDistance;
		}
	}
	//return index of closest point.
	return currentClosestIndex;
}

void Level::Render(std::string shader)
{
	//render the level
	_LevelTerrain->Render(shader);
	//this can be uncommented to show the track spline.
	//_LevelTrackSmoother->AddToBuffer();
	//_LevelTrackSmoother->Render();
	for (auto f : _FoliageBModelList) {
		if (shader == "") {
			ShaderManager::Instance()->GetShader(f->GetShader())->SetMat4("model", f->GetModelMatrix());
		}
		else {
			ShaderManager::Instance()->GetShader("shadow")->SetMat4("model", f->GetModelMatrix());
		}
		f->Render(shader);
	}
}

void Level::Update(float delta)
{
	_LapTimer->Update();
}
