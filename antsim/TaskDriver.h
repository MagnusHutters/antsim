#pragma once


#include "Vector2.h"
#include "TaskList.h"

//#include "Body.h"
#include "SensorDriver.h"
#include "ActionDriver.h"
#include "PheromoneDriver.h"


//enum actions { hold, followTrail, explore };

class Task
{
public:

	

	Task() : action(nullptr), sensor(nullptr), body(nullptr), pheromone(nullptr) {}

	void manageMetaData() {
		if (state != _oldState_) {
			newState = true;
			_oldState_ = state;
		}
		else {
			newState = false;
		}
		report = TaskReport();
	}

	virtual void init() {

	}

	virtual TaskReport calcTask() {
		//body->breakMotion();
		return TaskReport();
	}
	virtual void doTask() {}
	virtual void trySetJob(int jobHandle)
	{
		//throw("Should be overloaded");
	}

	bool newState=false;
	states::State _oldState_=states::None;
	states::State state = states::None;

protected:
	friend class Logger;

	Conditions currentCondition;

	TaskReport report = TaskReport();

	bool findJobPoint(const Conditions& conditions = Conditions()) {
		Vector2 jobPoint = sensor->senseJob(conditions);
		return (jobPoint.LengthSquared() > 0.1);
	}

	friend class TaskDriver;
	int avoidJobHandle=-1;


	SensorDriver* sensor;
	ActionDriver* action;
	//TaskDriver* taskDriver;
	BodyDriver* body;
	PheromoneDriver* pheromone;
	//void transferDrivers(SensorDriver* sensor, ActionDriver* action);

	
};


class TaskDriver
{
public:


	TaskDriver();
	TaskDriver(SensorDriver* sensor, ActionDriver* action, BodyDriver* body, PheromoneDriver* pheromoneDriver);



	void setTask(Task* newTask);

	TaskReport calcTask();
	void doTask();
	int getState() {
		return (int)task->state;
	}

	void trySetJob(int jobHandle)
	{
		task->trySetJob(jobHandle);
	}


private:
	friend class Logger;
	SensorDriver* sensor;
	ActionDriver* action;
	BodyDriver* body;
	PheromoneDriver* pheromoneDriver;


	Task* nextTask;

	Task* task;
	bool isNewTask = false;

};


