#include "TaskDriver.h"


TaskDriver::TaskDriver()
{
	task = new Task();
}

TaskDriver::TaskDriver(SensorDriver* sensor, ActionDriver* action, BodyDriver* body, PheromoneDriver* pheromoneDriver) :
	sensor(sensor), 
	action(action), 
	body(body), 
	pheromoneDriver(pheromoneDriver)
{
	task = new Task();
	task->action = action;
	task->sensor = sensor;
	task->body = body;
	task->pheromone = pheromoneDriver;
	//task->taskDriver = this;
}

void TaskDriver::setTask(Task* newTask)
{

	nextTask = newTask;
	isNewTask = true;
}

TaskReport TaskDriver::calcTask()
{
	return task->calcTask();
}

void TaskDriver::doTask()
{
	task->doTask();
	if (isNewTask) {
		isNewTask = false;

		nextTask->sensor = task->sensor;
		nextTask->action = task->action;
		nextTask->body = task->body;
		nextTask->pheromone = task->pheromone;
		//nextTask->taskDriver = this;


		delete(task);
		task = nextTask;

		task->init();
	}
}


