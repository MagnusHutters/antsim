#pragma once


#include "Vector2.h"
#include "Body.h"
#include "SensorDriver.h"

#include "ActionDriver.h"

#include "Pheromones2.h"

#include "TaskChanger.h"
#include "PheromoneDriver.h"
//#include "TaskFindJob.h"

#include "ActionFollowTrail.h"
#include "ActionExplore.h"
#include "EntityMap.h"
#include "Job.h"



class Ant
{

public:
	Ant(int id, int x, int y, float rot, PheromoneMap* pheromoneMap, EntityMap<Job>* jobMap);

	void process();
	void update();


	Body getBody();
	int getState()
	{
		return taskDriver->getState();
	}


	SensorDriver* sensorDriver;

	float val1, val2, val3;


private:

	EntityMap<Job>* jobMap;
	PheromoneMap* pheromoneMap;
	int id;
	
	BodyDriver* bodyDriver;
	ActionDriver* actionDriver;
	TaskChanger* taskDriver;
	PheromoneDriver* pheromoneDriver;


	//Body;





};

