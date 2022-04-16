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

	bool newState;
	states::State _oldState_;
	states::State state;

protected:
	

	TaskReport report = TaskReport();

	inline bool findJobPoint() {
		Vector2 jobPoint = sensor->senseJob(avoidJobHandle);
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


private:
	SensorDriver* sensor;
	ActionDriver* action;
	BodyDriver* body;
	PheromoneDriver* pheromoneDriver;


	Task* nextTask;

	Task* task;
	bool isNewTask = false;

};


