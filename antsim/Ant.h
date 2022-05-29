#pragma once


#include "Vector2.h"
#include "Body.h"
#include "SensorDriver.h"

#include "ActionDriver.h"

#include "Pheromones2.h"

#include "TaskChanger.h"
#include "PheromoneDriver.h"
//#include "TaskPheromoneController.h"

/*
#include "ActionFollowTrail.h"
#include "ActionExplore.h"
#include "CentralController.h"
#include "EntityMap.h"
#include "Job.h"*/
#include "MapContainer.h"
#include "RigidBodyMap.h"


class Ant
{

public:
	Ant(int id, int x, int y, float rot, MapContainer m);

	void process();
	void update();


	Body getBody();
	int getState()
	{
		return taskDriver->getState();
	}

	void trySetJob(int jobHandle)
	{
		taskDriver->trySetJob(jobHandle);
	}


	SensorDriver* sensorDriver;

	float val1, val2, val3;


private:
	friend class Logger;

	int id;
	
	BodyDriver* bodyDriver;
	ActionDriver* actionDriver;
	TaskChanger* taskDriver;
	PheromoneDriver* pheromoneDriver;

	MapContainer m;
	//Body;





};

